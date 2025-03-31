/**
 * @file trigger.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines the Triggers and Actions enums and their associated
 * conversions
 *
 * @version 0.1
 * @date 2025-03-25
 *
 * @see condition.h
 * @sa rule.h
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

#pragma once

#include <yaml-cpp/yaml.h>

#include <any>
#include <string>
#include <unordered_map>

namespace FlashBackClient
{
    enum Actions
    {
        no_action,
        download_changed,
        upload_changed,
        sync_files
    };

    enum Triggers
    {
        none,
        on_startup,
        on_shutdown,
        on_file_change,
        on_schedule,
        after_interval
    };
} // namespace FlashBackClient

namespace YAML
{
    // .as<T> functions for FlashBackClient::Actions
    template<>
    struct convert<FlashBackClient::Actions>
    {
        static bool decode(const Node& node, FlashBackClient::Actions& action)
        {
            if (!node.IsScalar()) return false;

            const std::string value = node.as<std::string>();

            if (value == "no_action")
                action = FlashBackClient::Actions::no_action;
            else if (value == "download_changed")
                action = FlashBackClient::Actions::download_changed;
            else if (value == "upload_changed")
                action = FlashBackClient::Actions::upload_changed;
            else if (value == "sync_files")
                action = FlashBackClient::Actions::sync_files;
            else
                return false;

            return true;
        }
    };

    // .as<T> functions for FlashBackClient::Triggers
    template<>
    struct convert<FlashBackClient::Triggers>
    {
        static bool decode(const Node& node, FlashBackClient::Triggers& trigger)
        {
            if (!node.IsScalar()) return false;

            const std::string value = node.as<std::string>();

            if (value == "none") trigger = FlashBackClient::Triggers::none;
            else if (value == "on_startup")
                trigger = FlashBackClient::Triggers::on_startup;
            else if (value == "on_shutdown")
                trigger = FlashBackClient::Triggers::on_shutdown;
            else if (value == "on_file_change")
                trigger = FlashBackClient::Triggers::on_file_change;
            else if (value == "on_schedule")
                trigger = FlashBackClient::Triggers::on_schedule;
            else if (value == "after_interval")
                trigger = FlashBackClient::Triggers::after_interval;
            else
                return false;

            return true;
        }
    };
} // namespace YAML
