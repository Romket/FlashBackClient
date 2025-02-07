#include <flashbackclient/scheduler.h>

#include <flashbackclient/defs.h>
#include <flashbackclient/logger.h>

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

        return true;
    }

    void Scheduler::Run()
    {
        Logger::LOG_INFO("Running scheduler");
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
    }

    void Scheduler::schedulerThread()
    {
        Logger::LOG_INFO("Schedular thread started");
        while (_running)
        {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _cv.wait(lock, [this] { return !_running || _flagged; });
            }

            if (!_running) break;
            _flagged = false;

            {
                std::unique_lock<std::mutex> targetsLock(_targetsMutex);
                for (const auto& target : _targets) target->CheckRules();
            }

            _cycleCount++;
        }
    }

    void Scheduler::loadTargets(const std::filesystem::path& path, int depth)
    {
        if (depth > RECURSION_LIMIT)
            throw std::invalid_argument("Too many directories");

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (std::filesystem::is_directory(entry.path()))
                loadTargets(entry.path(), depth + 1);
            else if (!std::filesystem::is_regular_file(entry.path()) ||
                     entry.path().extension() != ".yaml")
                continue;
            else
            {
                std::shared_ptr<Target> target = Target::Create(entry.path());

                if (!target->Initialize()) continue;
                _targets.push_back(std::move(target));
            }
        }
    }
} // namespace FlashBackClient
