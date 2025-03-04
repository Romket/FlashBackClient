#include <flashbackclient/scheduler.h>

#include <flashbackclient/condition.h>
#include <flashbackclient/defs.h>
#include <flashbackclient/logger.h>

#include <algorithm>
#include <chrono>
#include <croncpp.h>
#include <exception>
#include <memory>
#include <mutex>
#include <yaml-cpp/yaml.h>

namespace FlashBackClient
{
    bool Scheduler::Initialize()
    {
        if (!RuleManager::Initialize()) return false;

        loadTargets();

        for (const auto& target : _targets)
        {
            target->CheckRules({Triggers::on_startup});
        }

        return true;
    }

    bool Scheduler::Shutdown()
    {
        _running = false;
        _cv.notify_one();

        if (_schedulerThread.joinable()) _schedulerThread.join();

        LOG_TRACE("Scheduler thread joined");

        return true;
    }

    void Scheduler::Run()
    {
        LOG_INFO("Running scheduler");
        if (_running) return;

        _running         = true;
        _schedulerThread = std::thread([this]() { this->schedulerThread(); });
    }

    void Scheduler::Flag()
    {
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _flagged = true;
        }

        _cv.notify_one();

        LOG_TRACE("Scheduler flagged");
    }

    bool Scheduler::AddTimePoint(ScheduledTime& time, bool isNew)
    {
        LOG_INFO("Adding time point with cron: {}", time.cron);
        try
        {
            time.updated = false;

            auto        cron = cron::make_cron(time.cron);
            std::time_t now  = std::chrono::system_clock::to_time_t(
                std::chrono::system_clock::now());

            auto next = std::chrono::system_clock::from_time_t(
                cron::cron_next(cron, now));

            time.time = next;

            if (isNew) _times.push_back(time);

            std::sort(_times.begin(), _times.end(),
                      [](const ScheduledTime& a, const ScheduledTime& b) {
                          return a.time < b.time;
                      });

            _cv.notify_one();
        }
        catch (const std::exception& e)
        {
            LOG_ERROR("Failed to parse cron expression: {}", time.cron);
            LOG_ERROR("  {}", e.what());
            return false;
        }

        LOG_INFO("Time point added for time: {}",
                 time.time.time_since_epoch().count());

        return true;
    }

    bool Scheduler::ResetTimePoint(const Condition* target)
    {
        for (auto& time : _times)
        {
            if (time.owner == target)
            {
                if (!AddTimePoint(time, false)) return false;
            }
        }

        return true;
    }

    void Scheduler::SetTimeStatus(const Condition* owner, bool updated)
    {
        for (auto& time : _times)
        {
            if (time.owner == owner) time.owner->SetStatus(updated);
        }
    }

    void Scheduler::schedulerThread()
    {
        LOG_INFO("Scheduler thread started");
        std::unique_lock<std::mutex> lock(_mutex);

        while (_running)
        {
            if (_times.empty())
            {
                _cv.wait(lock, [this] { return !_running || !_times.empty(); });
            }
            else
            {
                auto now      = std::chrono::system_clock::now();
                auto nextTime = _times.front().time;

                if (now >= nextTime)
                {
                    auto task = _times.front();
                    _times.erase(_times.begin());

                    lock.unlock();
                    if (task.owner)
                    {
                        LOG_INFO("Checking rules for scheduled time");
                        task.owner->SetStatus(true);
                        task.updated = true;
                        task.owner->CheckParent();
                    }
                    lock.lock();
                }
                else
                {
                    _cv.wait_until(lock, nextTime, [this, nextTime] {
                        return !_running || (!_times.empty() &&
                                             _times.front().time != nextTime);
                    });
                }
            }
        }
        LOG_INFO("Exiting scheduler thread");
    }

    void Scheduler::loadTargets(const std::filesystem::path& path, int depth)
    {
        if (depth > RECURSION_LIMIT)
            throw std::invalid_argument("Too many directories");

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (std::filesystem::is_directory(entry.path()))
            {
                try
                {
                    loadTargets(entry.path(), depth + 1);
                }
                catch (const std::exception& e)
                {
                    LOG_ERROR("{}", e.what());
                    continue;
                }
            }
            else if (!std::filesystem::is_regular_file(entry.path()) ||
                     entry.path().extension() != ".yaml")
            {
                LOG_WARN("Invalid file at {}", entry.path().string());
                continue;
            }
            else
            {
                std::unique_ptr<Target> target = Target::Create(entry.path());

                if (!target->Initialize()) continue;
                _targets.push_back(std::move(target));
            }
        }
    }

    Triggers Scheduler::triggerFromScheduledTime(const ScheduledRuleType& time)
    {
        switch (time)
        {
            case ScheduledRuleType::atTimePoint: return Triggers::on_schedule;
            case ScheduledRuleType::afterLast:
            case ScheduledRuleType::beforeNext: return Triggers::after_interval;
            default: return Triggers::none;
        }
    }
} // namespace FlashBackClient
