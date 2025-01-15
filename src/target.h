#pragma once

#include <flashbackclient/managers/rulemanager.h>
#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/trigger.h>

#include <filesystem>
#include <unordered_map>
#include <vector>

namespace FlashBackClient
{
    class Target : public RuleManager, public SettingManager
    {
    public:
        Target(const std::filesystem::path& path) : RuleManager(path), SettingManager(path) {}
        virtual ~Target() = default;

    private:
        void afterCheck() override;

        bool upload();
        bool download();

        std::string _targetName;

        std::unordered_map<std::string, std::any> _targetSettings;
        std::unordered_map<std::string, Rule> _targetRules;
    };
} //namespace FlashBackClient
