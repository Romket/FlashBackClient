#include "target.h"

#include <flashbackclient/service_locator.h>
#include <listener/platform_listener.h>

#include <iostream>

namespace FlashBackClient
{
    bool Target::Initialize()
    {
        if (!ServiceLocator::IsProvided<PlatformListener>())
            ServiceLocator::Provide<PlatformListener>(new PlatformListener());

        auto rules = checkOverrideRules();
        rules.insert(_rules.begin(), _rules.end());

        for (const auto& rule : rules)
        {
            std::vector<Condition> ruleConditions = rule.first.Conditions;

            for (const auto& condition : ruleConditions)
            {
                if (condition.TriggerName == Triggers::on_file_change)
                {
                    if (!ServiceLocator::Get<PlatformListener>()->
                        AddListener(GetSettingValue<std::string>("path")))
                        return false;
                    return true;
                }
            }
        }

        return true;
    }

    void Target::afterCheck(const std::vector<Triggers>& givenTriggers)
    {
        auto rules = checkOverrideRules();
        rules.insert(_rules.begin(), _rules.end());

        for (const auto& rule : rules)
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
        // TODO: use std::none_of
        // cppcheck-suppress useStlAlgorithm
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
