#include "target.h"

#include <flashbackclient/service_locator.h>

#include <iostream>

namespace FlashBackClient
{
    bool Target::Initialize()
    {
        return RuleManager::Initialize() && SettingManager::Initialize();
    }

    void Target::afterCheck()
    {
        auto metDefaults = checkOverrideRules();

        auto rules = metDefaults;
        rules.insert(_rules.begin(), _rules.end());

        for (auto& rule : rules)
        {
            if (!rule.second)
                continue;

            if (rule.first.Action == Actions::upload_changed || rule.first.Action == Actions::sync_files)
                upload();
            if (rule.first.Action == Actions::download_changed || rule.first.Action == Actions::sync_files)
                download();
        }
    }

    std::unordered_map<Rule, bool> Target::checkOverrideRules()
    {
        auto schedulerRules = ServiceLocator::Get<RuleManager>()->GetRules();

        if (_settings.find("override_rules") == _settings.end())
            return schedulerRules;

        auto overrideRules = std::any_cast<std::vector<int>&>(_settings["override_rules"]);
    
        std::unordered_map<Rule, bool> metDefaults;

        for (const auto& schedulerRule : schedulerRules)
        {
            if (checkRule(schedulerRule.first, overrideRules))
                metDefaults.insert(std::pair<Rule, bool>(schedulerRule.first, true));
        }

        return metDefaults;
    }

    bool Target::checkRule(Rule defaultRule, const std::vector<int>& overrideRules)
    {
        for (int overrideRule : overrideRules)
        {
            if (overrideRule == defaultRule.id)
                return false;
        }

        return true;
    }

    bool Target::upload()
    {
        // TODO
        return true;
    }

    bool Target::download()
    {
        // TODO
        return true;
    }
} //namespace FlashBackClient