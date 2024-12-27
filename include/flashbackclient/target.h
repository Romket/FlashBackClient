#pragma once

#include <flashbackclient/trigger.h>

#include <unordered_map>
#include <vector>

namespace FlashBackClient
{
    class Target
    {
    public:
        void Upload();
        void Download();

    private:
        std::unordered_map<std::string, any> _targetSettings;
        std::vector<Rule> _targetRules;
    };
} //namespace FlashBackClient
