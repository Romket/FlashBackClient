#include <flashbackclient/managers/settingmanager.h>

#include <yaml-cpp/yaml.h>

#include <iostream>
#include <stdexcept>

namespace FlashBackClient
{
    SettingManager::SettingManager(const std::filesystem::path& path)
    {
        loadSettings(path);
    }

    void SettingManager::loadSettings(const std::filesystem::path& path)
    {
        std::cout << "Loading settings from path " << path.string() << std::endl;

        YAML::Node config = YAML::LoadFile(path.string());
        if (!config)
        {
            std::cout << "Failed to load config file" << std::endl;
            return;
        }

        if (config["name"])
            _settings["name"] = config["name"].as<std::string>();

        if (config["path"])
            _settings["path"] = config["path"].as<std::string>();

        if (config["encrypt"])
            _settings["encrypt"] = config["encrypt"].as<std::string>();

        if (config["override_rules"])
        {
            auto& rules = std::any_cast<std::vector<int>&>(_settings["override_rules"]);
            
            for (const auto& rule : config["override_rules"])
            {
                if (!rule["id"])
                    continue;

                if (_settings.find("override_rules") == _settings.end())
                    _settings["override_rules"] = std::vector<int> {};
                
                rules.push_back(rule["id"].as<int>());
            }
        }
    }
} //namespace FlashBackClient