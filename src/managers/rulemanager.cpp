/**
 * @file rulemanager.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief RuleManager implementation
 * @version 0.1
 * @date 2025-03-25
 *
 * @see rulemanager.h
 * @sa scheduler.h
 * @sa target.h
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <flashbackclient/logging/logger.h>
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
