#include <flashbackclient/target.h>

#include <flashbackclient/logger.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>
#include <listener/platform_listener.h>

namespace FlashBackClient
{
    bool Target::Initialize()
    {
        if (!RuleManager::Initialize() || !SettingManager::Initialize())
            return false;

        auto activeDefaults = checkOverrideRules();
        _rules.insert(activeDefaults.begin(), activeDefaults.end());

        if (!ServiceLocator::IsProvided<PlatformListener>())
        {
            ServiceLocator::Provide<PlatformListener>(new PlatformListener());
        }

        for (const auto& rule : _rules)
        {
            std::vector<Condition> ruleConditions = rule.first.Conditions;

            for (const auto& condition : ruleConditions)
            {
                if (condition.TriggerName == Triggers::on_file_change)
                {
                    ListenerInfo info;
                    info.Owner = shared_from_this();
                    info.Path  = GetSettingValue<std::string>("path");

                    if (!ServiceLocator::Get<PlatformListener>()->AddListener(
                            info))
                        return false;
                    return true;
                }
            }
        }

        return true;
    }

    bool Target::Shutdown() { return true; }

    void Target::CheckRules(const std::vector<Triggers>& givenTriggers)
    {
        for (const auto& rule : _rules)
        {
            if (rule.first.Conditions.empty()) continue;

            _rules.find(rule.first)->second =
                checkConditions(rule.first, givenTriggers);
        }

        afterCheck();
    }

    bool Target::checkConditions(const Rule&                  rule,
                                 const std::vector<Triggers>& givenTriggers)
    {
        for (const auto& condition : rule.Conditions)
        {
            if (checkTrigger(condition.TriggerName, givenTriggers)) continue;

            // TODO: nontrivial conditions
            if (condition.TriggerName == Triggers::on_file_change)
            {
                if (ServiceLocator::Get<PlatformListener>()->ListenerExists(
                        GetSettingValue<std::string>("path")) &&
                    checkFileChanges(condition))
                {
                    continue;
                }
            }

            return false;
        }

        for (const auto& condition : rule.Conditions)
        {
            if (condition.TriggerName == Triggers::on_file_change)
            {
                const std::filesystem::path& path =
                    GetSettingValue<std::string>("path");
                ServiceLocator::Get<PlatformListener>()->SetListenerStatus(
                    path, StatusEnum::active);
            }
        }

        Logger::LOG_INFO("Condition met");
        return true;
    }

    bool Target::checkFileChanges(const Condition& condition)
    {
        const std::filesystem::path& path =
            GetSettingValue<std::string>("path");

        // cppcheck-suppress useStlAlgorithm
        for (const auto& listener :
             ServiceLocator::Get<PlatformListener>()->GetListeners())
        {
            if (listener.Path == path &&
                listener.Status == StatusEnum::modified)
            {
                return true;
            }
        }
        return false;
    }

    bool Target::checkTrigger(Triggers                     trigger,
                              const std::vector<Triggers>& givenTriggers)
    {
        // TODO: use std::any_of
        // cppcheck-suppress useStlAlgorithm
        for (const auto& givenTrigger : givenTriggers)
        {
            if (givenTrigger == trigger) return true;
        }

        return false;
    }

    void Target::afterCheck(const std::vector<Triggers>& givenTriggers)
    {
        for (const auto& rule : _rules)
        {
            if (!rule.second) continue;

            if (rule.first.Action == Actions::upload_changed ||
                rule.first.Action == Actions::sync_files)
                upload();

            if (rule.first.Action == Actions::download_changed ||
                rule.first.Action == Actions::sync_files)
                download();
        }
    }

    std::unordered_map<Rule, bool> Target::checkOverrideRules()
    {
        auto schedulerRules = ServiceLocator::Get<Scheduler>()->GetRules();

        if (_settings.find("override_rules") == _settings.end())
            return schedulerRules;

        auto overrideRules =
            std::any_cast<std::vector<int>&>(_settings["override_rules"]);

        std::unordered_map<Rule, bool> metDefaults;

        for (const auto& schedulerRule : schedulerRules)
        {
            if (checkRule(schedulerRule.first, overrideRules))
            {
                metDefaults.insert(
                    std::pair<Rule, bool>(schedulerRule.first, true));
            }
        }

        return metDefaults;
    }

    bool Target::checkRule(Rule                    defaultRule,
                           const std::vector<int>& overrideRules)
    {
        // TODO: use std::none_of
        // cppcheck-suppress useStlAlgorithm
        for (int overrideRule : overrideRules)
        {
            if (overrideRule == defaultRule.id) return false;
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
} // namespace FlashBackClient
