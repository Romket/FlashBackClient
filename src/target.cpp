#include <flashbackclient/target.h>
#include <flashbackclient/helper.h>

#include <iostream>

namespace FlashBackClient
{
    Target::Target(const std::filesystem::path& configPath)
    {
        _targetRules = Helper::LoadRules(configPath.string());
        _targetSettings = Helper::LoadSettings(configPath.string());

        if (!_targetSettings["name"])
            throw std::invalid_argument("Target has no name");

        _targetName = _targetSettings["name"].as<std::string>();
    }

    void Target::CheckRules(Args... args)
    {
        for (const auto& rule : _targetRules)
        {
            if (rule.second.Conditions.empty())
                continue;

            if (checkConditions(rule.second, args...))
            {
                if (rule.second.Action == Actions::upload_changed)
                    upload();
                else if (rule.second.Action == Actions::download_changed)
                    download();
                else if (rule.second.Action == Actions::sync_files)
                {   
                    upload();
                    download();
                }
            }
        }
    }

    bool Target::checkConditions(const Rule& rule, Args... args)
    {
        for (const auto& condition : rule.Conditions)
        {
            if (((args == condition) || ...))
                continue;
            else if (condition.TriggerName == Triggers::on_file_change)
            {
                // TODO
                continue;
            }
            else if (condition.TriggerName == Triggers::on_schedule)
            {
                // TODO
                continue;
            }
            else if (condition.TriggerName == Triggers::after_interval)
            {
                // TODO
                continue;
            }

            return false;
        }

        return true;
    }

    bool Target::upload()
    {
        // TODO
        return true;
    }

    bool Target::download()
    {
        // TODO
        return true;
    }
} //namespace FlashBackClient