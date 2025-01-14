#pragma once

#include <flashbackclient/managers/rulemanager.h>
#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/trigger.h>

#include <filesystem>
#include <unordered_map>
#include <vector>

namespace FlashBackClient
{
    class Target : private RuleManager, private SettingManager
    {
    public:
        Target(const std::filesystem::path& path) : RuleManager(path), SettingManager(path) {}

    private:
        void afterCheck() override;

        bool upload();
        bool download();

        std::string _targetName;

        std::unordered_map<std::string, any> _targetSettings;
        std::unordered_map<std::string, Rule> _targetRules;
    };
} //namespace FlashBackClient
