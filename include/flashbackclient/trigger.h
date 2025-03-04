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
