#pragma once

#include <flashbackclient/trigger.h>

#include <unordered_map>
#include <vector>

namespace FlashBackClient
{
    class Target
    {
    public:
        Target(std::string& configPath);

        void Upload();
        void Download();

    private:
        void checkRules();

        std::string _targetName;

        std::unordered_map<std::string, any> _targetSettings;
        std::unordered_map<std::string, Rule> _targetRules;
    };
} //namespace FlashBackClient
