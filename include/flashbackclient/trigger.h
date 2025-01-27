#pragma once

#include <yaml-cpp/yaml.h>

#include <any>
#include <string>
#include <unordered_map>
#include <vector>

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

    //TODO: something better
    struct Condition
    {
        // cppcheck-suppress unusedStructMember
        Triggers TriggerName;
        // cppcheck-suppress unusedStructMember
        std::unordered_map<std::string, std::any> TriggerInfo;
    };

    struct Rule
    {
        int id;
        std::string name;
        // cppcheck-suppress unusedStructMember
        Actions Action;
        // cppcheck-suppress unusedStructMember
        std::vector<Condition> Conditions;

        bool operator==(const Rule& other) const
        {
            return id == other.id && name == other.name;
        }
    };
} //namespace FlashBackClient

// Hash function for Rule
namespace std
{
    template <>
    struct hash<FlashBackClient::Rule>
    {
        std::size_t operator()(const FlashBackClient::Rule& rule) const
        {
            return std::hash<int>()(rule.id) ^ (std::hash<std::string>()(rule.name) << 1);
        }
    };
} //namespace std

namespace YAML
{
    // .as<T> functions for FlashBackClient::Actions
    template<>
    struct convert<FlashBackClient::Actions>
    {
        static bool decode(const Node &node, FlashBackClient::Actions &action)
        {
            if (!node.IsScalar())
                return false;

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
        static bool decode(const Node &node, FlashBackClient::Triggers &trigger)
        {
            if (!node.IsScalar())
                return false;

            const std::string value = node.as<std::string>();

            if (value == "none")
                trigger = FlashBackClient::Triggers::none;
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
} //namespace YAML
