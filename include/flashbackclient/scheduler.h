#pragma once

#include <flashbackclient/target.h>
#include <flashbackclient/trigger.h>

#include <vector>

#DEFINE CONFIG_DIR "~/.config/flashbackclient/scheduler"
#DEFINE CONFIG_FILE "scheduler.yaml"

namespace FlashBackClient
{
    class Scheduler
    {
    public:
        Scheduler() : _globalRules(Helper::LoadRules(CONFIG_DIR "/" CONFIG_FILE)) {}

    private:
        void checkTargets();

        std::vector<Target> _targets;
        std::vector<Rule> _globalRules;
    };
} //namespace FlashBackClient