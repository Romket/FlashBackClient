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
        explicit Target(const std::filesystem::path& path) : RuleManager(path), SettingManager(path) {}
        virtual ~Target() = default;

        bool Initialize() override;

    private:
        void afterCheck(const std::vector<Triggers>& givenTriggers = {}) override;
        std::unordered_map<Rule, bool> checkOverrideRules();
        bool checkRule(Rule defaultRule, const std::vector<int>& overrideRules);

        bool upload();
        bool download();
    };
} //namespace FlashBackClient
