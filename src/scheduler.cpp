#include <flashbackclient/scheduler.h>

#include <yaml-cpp/yaml.h>

#DEFINE CONFIG_DIR "~/.config/flashbackclient/scheduler"
#DEFINE CONFIG_FILE "scheduler.yaml"

namespace FlashBackClient
{
    void Scheduler::checkTargets()
    {
        // TODO
    }

    void Scheduler::loadDefaultRules()
    {
        YAML::Node schedulerConfig = YAML::LoadFile(CONFIG_DIR "/" CONFIG_FILE);
        
        if (!schedulerConfig["rules"])
        {
            std::cout << "No rules found in config file" << std::endl;
            return;
        }

        for (const auto& rule : schedulerConfig["rules"])
        {
            if (!rule["name"])
            {
                std::cout << "Rule has no name" << std::endl;
                continue;
            }

            name = rule["name"].as<std::string>();

            Rule newRule;
            if (!rule["action"] || !rule["action"].as<Actions>(newRule.Action);)
            {
                std::cout << "Invalid or no action in rule" << name << std::endl;
                continue;
            }

            if (!loadCases(newRule, rule["cases"]);)
            {
                std::cout << "Failed to load cases for rule" << name << std::endl;
                continue;
            }

            _globalRules.push_back(std::pair<std::string, Rule>(name, newRule));
        }
    }

    bool Scheduler::loadCases(Rule& rule, const YAML::Node& casesNode)
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

                    newCase.TriggerInfo["times"].push_back(time["cron_exp"].as<std::string>());
                }
            }
            elif (trigger == Triggers::after_interval)
            {
                if (!caseNode["after_last"])
                    after_last = 0;
                else
                    after_last = caseNode["after_last"].as<int>();

                if (!caseNode["before_next_scheduled"])
                    before_next_scheduled = 0;
                else
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