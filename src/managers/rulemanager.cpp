#include <flashbackclient/managers/rulemanager.h>

#include <iostream>

namespace FlashBackClient
{
    bool RuleManager::Initialize()
    {
        loadRules(_ruleFile);

        return true;
    }

    void RuleManager::loadRules(const std::filesystem::path& path)
    {
        std::cout << "Loading rules from path " << path.string() << std::endl;

        YAML::Node config = YAML::LoadFile(path.string());
        if (!config)
        {
            std::cout << "Failed to load config file" << std::endl;
            return;
        }

        if (!config["rules"])
        {
            std::cout << "No rules found in config file" << std::endl;
            return;
        }

        for (const auto& rule : config["rules"])
        {
            if (!rule["id"])
            {
                std::cout << "Rule has no id" << std::endl;
                continue;
            }

            Rule newRule;
            newRule.id = rule["id"].as<int>();

            if (!rule["name"])
            {
                std::cout << "Rule has no name" << std::endl;
                continue;
            }

            newRule.name = rule["name"].as<std::string>();

            if (!rule["action"] || !rule["action"].as<Actions>(newRule.Action))
            {
                std::cout << "Invalid or no action in rule \"" << newRule.name << '\"' << std::endl;
                continue;
            }

            if (!loadCases(newRule, rule["cases"]))
            {
                std::cout << "Failed to load cases for rule \"" << newRule.name << '\"' << std::endl;
                continue;
            }

            _rules.insert(std::pair<Rule, bool>(newRule, false));
        }
    }

    bool RuleManager::loadCases(Rule& rule, const YAML::Node& casesNode)
    {
        if (!casesNode)
            return false;

        for (const auto& caseNode : casesNode)
        {
            if (!caseNode["id"])
            {
                std::cout << "Case has no id" << std::endl;
                continue;
            }

            Triggers trigger = caseNode["id"].as<Triggers>();

            if (trigger == Triggers::none)
                continue;

            Condition newCase;
            newCase.TriggerName = trigger;

            if (trigger == Triggers::on_schedule)
            {
                if (!caseNode["times"])
                {
                    std::cout << "Case has no times" << std::endl;
                    continue;
                }

                for (const auto& time : caseNode["times"])
                {
                    if (!time["cron_exp"])
                    {
                        std::cout << "Time has no cron_exp" << std::endl;
                        continue;
                    }

                    if (newCase.TriggerInfo.find("times") == newCase.TriggerInfo.end()) {
                        newCase.TriggerInfo["times"] = std::vector<std::string>();
                    }

                    auto& times = std::any_cast<std::vector<std::string>&>(newCase.TriggerInfo["times"]);
                    times.push_back(time["cron_exp"].as<std::string>());
                }
            }
            else if (trigger == Triggers::after_interval)
            {
                int after_last = 0;
                int before_next_scheduled = 0;

                if (caseNode["after_last"])
                    after_last = caseNode["after_last"].as<int>();
                if (caseNode["before_next_scheduled"])
                    before_next_scheduled = caseNode["before_next_scheduled"].as<int>();

                newCase.TriggerInfo["after_last"] = after_last;
                newCase.TriggerInfo["before_next_scheduled"] = before_next_scheduled;
            }

            rule.Conditions.push_back(newCase);
        }

        if (rule.Conditions.empty())
        {
            std::cout << "Rule has no cases" << std::endl;
            return false;
        }

        return true;
    }
} //namespace FlashBackClient
