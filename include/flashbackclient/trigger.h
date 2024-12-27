#pragma once

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

    struct Condition
    {
        Triggers TriggerName;
        std::unordered_map<std::string, any> TriggerInfo;
    };

    struct Rule
    {
        Actions Action;
        std::vector<Condition> Conditions;
    };
} //namespace FlashBackClient

namespace YAML
{
    template<>
    struct convert<Actions>
    {
        static bool decode(const Node &node, Actions &action)
        {
            if (!node.IsScalar())
                return false;

            const std::string value = node.as<std::string>();
            switch (value)
            {
            case "no_action":
                action = no_action;
                break;
            case "download_changed":
                action = download_changed;
                break;
            case "upload_changed":
                action = upload_changed;
                break;
            case "sync_files":
                action = sync_files;
                break;
            default:
                return false;
            }

            return true;
        }
    };

    template<>
    struct convert<Triggers>
    {
        static bool decode(const Node &node, Triggers &trigger)
        {
            if (!node.IsScalar())
                return false;

            const std::string value = node.as<std::string>();
            switch (value)
            {
            case "none":
                trigger = none;
                break;
            case "on_startup":
                trigger = on_startup;
                break;
            case "on_shutdown":
                trigger = on_shutdown;
                break;
            case "on_file_change":
                trigger = on_file_change;
                break;
            case "on_schedule":
                trigger = on_schedule;
                break;
            case "after_interval":
                trigger = after_interval;
                break;
            default:
                return false;
            }

            return true;
        }
    };
} //namespace YAML