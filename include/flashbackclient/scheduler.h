#pragma once
#include <flashbackclient/defs.h>

#include <flashbackclient/target.h>
#include <flashbackclient/trigger.h>

#include <filesystem>
#include <vector>

namespace FlashBackClient
{
    class Scheduler
    {
    public:
        Scheduler() : _globalRules(Helper::LoadRules(SCHEDULER_CONFIG_FILE_PATH)) { loadTargets(); }

        ~Scheduler() = default;

    private:
        void checkStartup();

        void loadTargets(const std::filesystem::path& path, int depth = 0);

        std::vector<Target> _targets;
        std::vector<Rule> _globalRules;
    };
} //namespace FlashBackClient