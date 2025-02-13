#pragma once

#include <chrono>
#include <flashbackclient/managers/rulemanager.h>

#include <flashbackclient/defs.h>
#include <flashbackclient/target.h>
#include <flashbackclient/trigger.h>

#include <atomic>
#include <condition_variable>
#include <filesystem>
#include <memory>
#include <thread>
#include <vector>

namespace FlashBackClient
{
    enum class ScheduledRuleType
    {
        atTimePoint,
        afterLast,

        // TODO
        beforeNext
    };

    struct ScheduledTime
    {
        std::chrono::time_point<std::chrono::system_clock> Time;
        // cppcheck-suppress unusedStructMember
        ScheduledRuleType Type;
        // cppcheck-suppress unusedStructMember
        std::string             Cron;
        std::shared_ptr<Target> Owner;
    };

    class Scheduler : public RuleManager
    {
    public:
        Scheduler() : RuleManager(SCHEDULER_CONFIG_FILE_PATH) {}
        virtual ~Scheduler() = default;

        bool Initialize() override;
        bool Shutdown() override;

        void Run();

        void Flag();
        bool AddTimePoint(ScheduledTime& time);

    private:
        void loadTargets(const std::filesystem::path& path  = TARGET_DEF_DIR,
                         int                          depth = 0);

        void schedulerThread();

        Triggers triggerFromScheduledTime(const ScheduledRuleType& time);

        // cppcheck-suppress unusedStructMember
        std::vector<std::shared_ptr<Target>> _targets;
        std::mutex                           _targetsMutex;

        std::atomic<bool> _running;
        std::atomic<bool> _flagged;

        std::thread             _schedulerThread;
        std::condition_variable _cv;
        std::mutex              _mutex;

        // cppcheck-suppress unusedStructMember
        std::vector<ScheduledTime> _times;

        uint32_t _cycleCount = 0;
    };
} // namespace FlashBackClient
