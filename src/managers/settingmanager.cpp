/**
 * @file settingmanager.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief
 * @version 0.1
 * @date 2025-03-25
 *
 * @see settingmanager.h
 * @sa configs.h
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

#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/logging/logger.h>
#include <yaml-cpp/yaml.h>

namespace FlashBackClient
{
    bool SettingManager::Initialize()
    {
        loadSettings(_settingFile);

        return true;
    }

    bool SettingManager::Shutdown() { return true; }

    void SettingManager::loadSettings(const std::filesystem::path& path)
    {
        LOG_INFO("Loading settings from path {}", path.string());

        YAML::Node config = YAML::LoadFile(path.string());
        if (!config)
        {
            LOG_ERROR("Failed to load config file");
            return;
        }

        if (config["name"])
            _settings["name"] = config["name"].as<std::string>();

        if (config["path"])
            _settings["path"] =
                expandHomeDirectory(config["path"].as<std::string>());

        if (config["encrypt"])
            _settings["encrypt"] = config["encrypt"].as<std::string>();

        if (config["override_rules"])
        {
            _settings["override_rules"] = std::vector<int> {};
            auto& rules =
                std::any_cast<std::vector<int>&>(_settings["override_rules"]);

            for (const auto& rule : config["override_rules"])
            {
                if (!rule["id"]) continue;
                rules.push_back(rule["id"].as<int>());
            }
        }

        if (config["path_ignores"])
        {
            _settings["path_ignores"] = std::vector<std::string> {};
            auto& ignores = std::any_cast<std::vector<std::string>&>(
                _settings["path_ignores"]);

            for (const auto& ignore : config["path_ignores"])
            {
                if (!ignore["ignore"]) continue;
                ignores.push_back(ignore["ignore"].as<std::string>());
            }
        }
    }

    std::string SettingManager::expandHomeDirectory(const std::string& path)
    {
        if (path.empty() || path[0] != '~') return path;

        const char* homeDir = std::getenv("HOME");
        if (homeDir == nullptr)
        {
            LOG_ERROR("HOME environment variable is not set.");
            return path; // Return original path if HOME is not set
        }

        std::string expandedPath = homeDir;
        expandedPath += path.substr(1); // Append the rest of the path after '~'
        return expandedPath;
    }
} // namespace FlashBackClient
