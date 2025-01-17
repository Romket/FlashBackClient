#pragma once

#include <flashbackclient/managers/rulemanager.h>

#include <flashbackclient/defs.h>

#include <flashbackclient/trigger.h>

#include <filesystem>
#include <memory>
#include <vector>

#include "target.h"

namespace FlashBackClient
{
    class Scheduler : public RuleManager
    {
    public:
        Scheduler() : RuleManager(SCHEDULER_CONFIG_FILE_PATH) {}
        virtual ~Scheduler() = default;

        bool Initialize() override;

    private:
        void afterCheck() override;

        void loadTargets(const std::filesystem::path& path = TARGET_DEF_DIR, int depth = 0);

        std::vector<std::unique_ptr<Target>> _targets;
        std::vector<Rule> _globalRules;
    };
} //namespace FlashBackClient