/**
 * @file configs.cpp
 * @author Chase Attebury (Appleberry) (chaseappleberryboi@gmail.com)
 * @brief Implements the ConfigManager for handling global configs
 * @version 0.1
 * @date 2025-04-01
 *
 * @see configs.h
 * @see settingmanager.h
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

#include <filesystem>
#include <fstream>

#include <flashbackclient/configdefs.h>
#include <flashbackclient/configs.h>
#include <flashbackclient/logging/logger.h>

namespace FlashBackClient
{
    bool ConfigManager::Initialize() { return SettingManager::Initialize(); }
    bool ConfigManager::Shutdown() { return SettingManager::Shutdown(); }

    void ConfigManager::GenerateConfigs()
    {
        if (!std::filesystem::exists(CONFIG_FILE_PATH))
        {
            LOG_INFO("No global config found");
            if (!std::filesystem::exists(CONFIG_DIR))
            {
                LOG_INFO("{} not found, creating now", CONFIG_DIR);
                std::filesystem::create_directory(CONFIG_DIR);
                LOG_INFO("{} created", CONFIG_DIR);
                LOG_INFO("Creating default config {} in {} now", CONFIG_FILE,
                         CONFIG_DIR);
                std::ofstream globalConfig(CONFIG_FILE_PATH);
                globalConfig.close();
                LOG_INFO("Default global config created at {}",
                         CONFIG_FILE_PATH);
            }
            else
            {
                LOG_INFO("{} found, creating default global config {} now",
                         CONFIG_DIR, CONFIG_FILE);
                std::ofstream globalConfig(CONFIG_FILE_PATH);

                globalConfig.close();
                LOG_INFO("Default global config created at {}",
                         CONFIG_FILE_PATH);
            }
        }
        else { LOG_INFO("Global config found in {}, continuing", CONFIG_DIR); }
    }
} // namespace FlashBackClient
