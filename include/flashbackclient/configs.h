/**
 * @file configs.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines an inherited class of SettingManager for managing global
 * settings
 *
 * @version 0.1
 * @date 2025-03-28
 *
 * @see configs.cpp
 * @see settingmanager.h
 * @sa settingmanager.cpp
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This file is part of FlashBackClient
 *
 * FlashBackClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FlashBackClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FlashBackClient.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/defs.h>

namespace FlashBackClient
{
    class ConfigManager : public SettingManager
    {
    public:
        inline ConfigManager() : SettingManager(CONFIG_FILE_PATH) {}

        bool Initialize() override;
        bool Shutdown() override;

        static void GenerateConfigs();
    };
} // namespace FlashBackClient
