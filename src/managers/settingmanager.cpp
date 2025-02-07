#include <flashbackclient/logger.h>
#include <flashbackclient/managers/settingmanager.h>

#include <yaml-cpp/yaml.h>

namespace FlashBackClient
{
    bool SettingManager::Initialize()
    {
        loadSettings(_settingFile);

        return true;
    }

    bool SettingManager::Shutdown() { return true; }

    void SettingManager::loadSettings(const std::filesystem::path& path)
    {
        LOG_INFO("Loading settings from path {}", path.string());

        YAML::Node config = YAML::LoadFile(path.string());
        if (!config)
        {
            LOG_ERROR("Failed to load config file");
            return;
        }

        if (config["name"])
            _settings["name"] = config["name"].as<std::string>();

        if (config["path"])
            _settings["path"] =
                expandHomeDirectory(config["path"].as<std::string>());

        if (config["encrypt"])
            _settings["encrypt"] = config["encrypt"].as<std::string>();

        if (config["override_rules"])
        {
            auto& rules =
                std::any_cast<std::vector<int>&>(_settings["override_rules"]);

            for (const auto& rule : config["override_rules"])
            {
                if (!rule["id"]) continue;

                if (_settings.find("override_rules") == _settings.end())
                    _settings["override_rules"] = std::vector<int> {};

                rules.push_back(rule["id"].as<int>());
            }
        }
    }

    std::string SettingManager::expandHomeDirectory(const std::string& path)
    {
        if (path.empty() || path[0] != '~') return path;

        const char* homeDir = std::getenv("HOME");
        if (homeDir == nullptr)
        {
            LOG_ERROR("HOME environment variable is not set.");
            return path; // Return original path if HOME is not set
        }

        std::string expandedPath = homeDir;
        expandedPath += path.substr(1); // Append the rest of the path after '~'
        return expandedPath;
    }
} // namespace FlashBackClient
