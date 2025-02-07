#include <flashbackclient/logger.h>
#include <flashbackclient/managers/rulemanager.h>

namespace FlashBackClient
{
    bool RuleManager::Initialize()
    {
        loadRules(_ruleFile);

        return true;
    }

    bool RuleManager::Shutdown() { return true; }

    void RuleManager::loadRules(const std::filesystem::path& path)
    {
        LOG_INFO("Loading rules from path {}", path.string());

        YAML::Node config = YAML::LoadFile(path.string());
        if (!config)
        {
            LOG_ERROR("Failed to load config file");
            return;
        }

        if (!config["rules"])
        {
            LOG_WARN("No rules found in config file");
            return;
        }

        for (const auto& rule : config["rules"])
        {
            if (!rule["id"])
            {
                LOG_WARN("Rule has no id");
                continue;
            }

            Rule newRule;
            newRule.id = rule["id"].as<int>();

            if (!rule["name"])
            {
                LOG_WARN("Rule has no name");
                continue;
            }

            newRule.name = rule["name"].as<std::string>();

            if (!rule["action"] || !rule["action"].as<Actions>(newRule.Action))
            {
                LOG_WARN("Invalid or no action in rule \"{}",
                                 newRule.name);
                continue;
            }

            if (!loadCases(newRule, rule["cases"]))
            {
                LOG_ERROR("Failed to load cases for rule \"{}",
                                  newRule.name);
                continue;
            }

            _rules.insert(std::pair<Rule, bool>(newRule, false));
        }
    }

    bool RuleManager::loadCases(Rule& rule, const YAML::Node& casesNode)
    {
        if (!casesNode) return false;

        for (const auto& caseNode : casesNode)
        {
            if (!caseNode["id"])
            {
                LOG_WARN("Case has no id");
                continue;
            }

            Triggers trigger = caseNode["id"].as<Triggers>();

            if (trigger == Triggers::none) continue;

            Condition newCase;
            newCase.TriggerName = trigger;

            if (trigger == Triggers::on_schedule)
            {
                if (!caseNode["times"])
                {
                    LOG_WARN("Case has no times");
                    continue;
                }

                for (const auto& time : caseNode["times"])
                {
                    if (!time["cron_exp"])
                    {
                        LOG_WARN("Time has no cron_exp");
                        continue;
                    }

                    if (newCase.TriggerInfo.find("times") ==
                        newCase.TriggerInfo.end())
                    {
                        newCase.TriggerInfo["times"] =
                            std::vector<std::string>();
                    }

                    auto& times = std::any_cast<std::vector<std::string>&>(
                        newCase.TriggerInfo["times"]);
                    times.push_back(time["cron_exp"].as<std::string>());
                }
            }
            else if (trigger == Triggers::after_interval)
            {
                int after_last            = 0;
                int before_next_scheduled = 0;

                if (caseNode["after_last"])
                    after_last = caseNode["after_last"].as<int>();
                if (caseNode["before_next_scheduled"])
                    before_next_scheduled =
                        caseNode["before_next_scheduled"].as<int>();

                newCase.TriggerInfo["after_last"] = after_last;
                newCase.TriggerInfo["before_next_scheduled"] =
                    before_next_scheduled;
            }

            rule.Conditions.push_back(newCase);
        }

        if (rule.Conditions.empty())
        {
            LOG_WARN("Rule has no cases");
            return false;
        }

        return true;
    }
} // namespace FlashBackClient
