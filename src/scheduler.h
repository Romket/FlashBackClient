#pragma once

#include <flashbackclient/managers/rulemanager>

#include <flashbackclient/defs.h>

#include <flashbackclient/trigger.h>

#include <filesystem>
#include <unique_pointer>
#include <vector>

#include "target.h"

namespace FlashBackClient
{
    class Scheduler : private RuleManager
    {
    public:
        Scheduler() : RuleManager(SCHEDULER_CONFIG_FILE_PATH) { loadTargets(); }

    private:
        void afterCheck() override;

        void checkStartup();

        void loadTargets(const std::filesystem::path& path = TARGET_DEF_DIR, int depth = 0);

        std::vector<std::unique_pointer<Target>> _targets;
        std::vector<Rule> _globalRules;
    };
} //namespace FlashBackClient