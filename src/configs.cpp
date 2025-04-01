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

    // Generates default global and scheduler configs if they are not present
    // Also creates example target definition if none are found
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

                LOG_INFO("Creating default global config {} in {} now", CONFIG_FILE,
                         CONFIG_DIR);
                std::ofstream globalConfig(CONFIG_FILE_PATH);
                globalConfig << DEFAULT_GLOBAL_CONFIG;
                globalConfig.close();
                LOG_INFO("Default global config created at {}",
                         CONFIG_FILE_PATH);
            }
            else
            {
                LOG_INFO("{} found, creating default global config {} now",
                         CONFIG_DIR, CONFIG_FILE);
                std::ofstream globalConfig(CONFIG_FILE_PATH);
                globalConfig << DEFAULT_GLOBAL_CONFIG;
                globalConfig.close();
                LOG_INFO("Default global config created at {}",
                         CONFIG_FILE_PATH);
            }
        }
        else { LOG_INFO("Global config found in {}, continuing", CONFIG_DIR); }

        if (!std::filesystem::exists(SCHEDULER_CONFIG_FILE_PATH))
        {
            LOG_INFO("No scheduler config found");
            if (!std::filesystem::exists(SCHEDULER_CONFIG_DIR))
            {
                LOG_INFO("{} not found, creating now", SCHEDULER_CONFIG_DIR);
                std::filesystem::create_directory(SCHEDULER_CONFIG_DIR);
                LOG_INFO("{} created", SCHEDULER_CONFIG_DIR);

                LOG_INFO("Creating default scheduler config {} in {} now", SCHEDULER_CONFIG_FILE,
                         SCHEDULER_CONFIG_DIR);
                std::ofstream schedulerConfig(SCHEDULER_CONFIG_FILE_PATH);
                schedulerConfig << DEFAULT_SCHEDULER_CONFIG;
                schedulerConfig.close();
                LOG_INFO("Default scheduler config created at {}",
                         SCHEDULER_CONFIG_FILE_PATH);
            }
            else
            {
                LOG_INFO("{} found, creating default scheduler config {} now",
                         SCHEDULER_CONFIG_DIR, SCHEDULER_CONFIG_FILE);
                std::ofstream schedulerConfig(SCHEDULER_CONFIG_FILE_PATH);
                schedulerConfig << DEFAULT_SCHEDULER_CONFIG;
                schedulerConfig.close();
                LOG_INFO("Default scheduler config created at {}",
                         SCHEDULER_CONFIG_FILE_PATH);
            }
        }
        else { LOG_INFO("Scheduler config found in {}, continuing", SCHEDULER_CONFIG_DIR); }

        if (!std::filesystem::exists(TARGET_DEF_DIR))
        {
            LOG_INFO("{} does not exist, creating directory", TARGET_DEF_DIR);
            std::filesystem::create_directory(TARGET_DEF_DIR);
            LOG_INFO("{} created", TARGET_DEF_DIR);

            LOG_INFO("Creating example target definition {}", EXAMPLE_TARGET_DEF_FILE);
            std::ofstream targetConfig(EXAMPLE_TARGET_DEF_FILE_PATH);
            targetConfig << DEFAULT_TARGET_CONFIG;
            targetConfig.close();
            LOG_INFO("Example target definition created at {}", EXAMPLE_TARGET_DEF_FILE_PATH);
        }
        else if (std::filesystem::is_empty(TARGET_DEF_DIR))
        {
            LOG_WARN("No target definitions detected in {}", TARGET_DEF_DIR);
            LOG_INFO("Creating example target definition example.yaml");
            std::ofstream targetConfig(EXAMPLE_TARGET_DEF_FILE_PATH);
            targetConfig << DEFAULT_TARGET_CONFIG;
            targetConfig.close();
            LOG_INFO("Example target definition created at {}", EXAMPLE_TARGET_DEF_FILE_PATH);
        }
        else { LOG_INFO("Target definition(s) found in {}, continuing", TARGET_DEF_DIR); }
    }
} // namespace FlashBackClient
