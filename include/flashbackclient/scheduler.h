#pragma once

#include <flashbackclient/target.h>
#include <flashbackclient/trigger.h>

#include <vector>

namespace FlashBackClient
{
    class Scheduler
    {
    private:
        void checkTargets();
        void loadDefaultRules();

        bool loadCases(Rule& rule, const YAML::Node& casesNode);

        std::vector<Target> _targets;
        std::vector<Rule> _globalRules;
    };
} //namespace FlashBackClient