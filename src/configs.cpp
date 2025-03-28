/**
 * @file configs.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Implements the ConfigManager for handling global configs
 * @version 0.1
 * @date 2025-03-28
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

#include <flashbackclient/configs.h>

namespace FlashBackClient
{
    bool ConfigManager::Initialize() { return SettingManager::Initialize(); }
    bool ConfigManager::Shutdown() { return SettingManager::Shutdown(); }

    void ConfigManager::GenerateConfigs()
    {
        // TODO
    }
} // namespace FlashBackClient
