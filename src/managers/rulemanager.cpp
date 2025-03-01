#include <flashbackclient/logger.h>
#include <flashbackclient/managers/rulemanager.h>
#include <memory>

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
            int id = rule["id"].as<int>();

            if (!rule["name"])
            {
                LOG_WARN("Rule has no name");
                continue;
            }

            std::string name = rule["name"].as<std::string>();

            Actions action;
            if (!rule["action"] || !rule["action"].as<Actions>(action))
            {
                LOG_WARN("Invalid or no action in rule \"{}", name);
                continue;
            }

            std::unique_ptr<Rule> newRule =
                std::make_unique<Rule>(id, name, action);
            if (!newRule->LoadCases(rule["cases"]))
            {
                LOG_ERROR("Failed to load cases for rule \"{}\"", name);
                continue;
            }

            _rules.push_back(std::move(newRule));
        }
    }
} // namespace FlashBackClient
