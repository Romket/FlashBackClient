#pragma once

#include <any>
#include <filesystem>
#include <string>
#include <unordered_map>

namespace FlashBackClient
{
    class SettingManager
    {
    public:
        inline SettingManager(const std::filesystem::path& path) : loadSettings(path) {}

        inline std::unordered_map<std::string, any> GetSettings() { return _settings; }
        inline void SetSettings(const std::unordered_map<std::string, any>& settings) { _settings = settings; }

        template<typename T>
        inline T GetSettingValue(const std::string& name)
        {
            if (_settings.find(name) != _settings.end())
                return _settings->second;

            throw std::invalid_argument("key not found in settings map");
        }

        inline void SetSettingValue(const std::string& name, any value)
        {
            _settings[name] = value;
        }

    protected:

    private:
        void loadSettings(const std::filesystem::path& path);

        std::unordered_map<std::string, any> _settings;
    };
} //namespace FlashBackClient