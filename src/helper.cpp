#include <flashbackclient/helper.h>

#include <iostream>

namespace FlashBackClient
{
    std::unordered_map<std::string, Rule> Helper::LoadRules(std::string& configPath)
    {
        std::unordered_map<std::string, Rule> rules;

        YAML::Node config = YAML::LoadFile(configPath);
        if (!config)
        {
            std::cout << "Failed to load config file" << std::endl;
            return rules;
        }
        
        if (!config["rules"])
        {
            std::cout << "No rules found in config file" << std::endl;
            return;
        }

        for (const auto& rule : config["rules"])
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

            rules.push_back(std::pair<std::string, Rule>(name, newRule));
        }

        return rules;
    }

    std::unordered_map<std::string, any> Helper::LoadSettings(std::string& configPath)
    {
        std::unordered_map<std::string, any> settings;

        YAML::Node config = YAML::LoadFile(configPath);
        if (!config)
        {
            std::cout << "Failed to load config file" << std::endl;
            return settings;
        }

        if (config["name"])
            settings["name"] = config["name"].as<std::string>();

        if (config["path"])
            settings["path"] = config["path"].as<std::string>();

        if (config["encrypt"])
            settings["encrypt"] = config["encrypt"].as<bool>();

        return settings;
    }

    bool Helper::loadCases(Rule& rule, const YAML::Node& casesNode)
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