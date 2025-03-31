/**
 * @file scheduler.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines the Scheduler class, which inherits from RuleManager and
 * handles default rules and cron scheduling
 *
 * @version 0.1
 * @date 2025-03-25
 *
 * @see scheduler.cpp
 * @see rulemanager.h
 * @see target.cpp
 * @sa rulemanager.cpp
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <flashbackclient/managers/rulemanager.h>

#include <flashbackclient/defs.h>
#include <flashbackclient/target.h>
#include <flashbackclient/trigger.h>

#include <atomic>
#include <chrono>
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
        std::chrono::time_point<std::chrono::system_clock> time;
        // cppcheck-suppress unusedStructMember
        ScheduledRuleType type;
        // cppcheck-suppress unusedStructMember
        std::string cron;
        // cppcheck-suppress unusedStructMember
        Condition* owner;
        bool       updated = false;
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
        bool AddTimePoint(ScheduledTime& time, bool isNew);
        bool ResetTimePoint(const Condition* owner);

        const std::vector<ScheduledTime>& GetScheduledTimes() const
        {
            return _times;
        }

        void SetTimeStatus(const Condition* owner, bool updated);

    private:
        void loadTargets(const std::filesystem::path& path  = TARGET_DEF_DIR,
                         int                          depth = 0);

        void schedulerThread();

        Triggers triggerFromScheduledTime(const ScheduledRuleType& time);

        // cppcheck-suppress unusedStructMember
        std::vector<std::unique_ptr<Target>> _targets;
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
