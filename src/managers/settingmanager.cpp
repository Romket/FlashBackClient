#include <flashbackclient/managers/settingmanager.h>

#include <stdexcept>

namespace FlashBackClient
{
    void SettingManager::loadSettings(const std::filesystem::path& path)
    {
        YAML::Node config = YAML::LoadFile(configPath.string());
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
            _settings["override_rules"] = std::vector<int> {};
            for (const auto& rule : config["override_rules"])
            {
                if (!rule["id"])
                    continue;

                _settings["override_rules"].push_back(rule["id"].as<int>());
            }
        }
    }
} //namespace FlashBackClient