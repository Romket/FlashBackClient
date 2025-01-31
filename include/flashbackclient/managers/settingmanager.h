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
        explicit SettingManager(const std::filesystem::path& path) : _settingFile(path) {}
        virtual ~SettingManager() = default;

        virtual bool Initialize();

        const inline std::unordered_map<std::string, std::any>& GetSettings() const { return _settings; }

        inline void SetSettings(const std::unordered_map<std::string, std::any>& settings) { _settings = settings; }

        template<typename T>
        inline T GetSettingValue(const std::string& name) const
        {
            if (_settings.find(name) != _settings.end())
                return std::any_cast<T>(_settings.at(name));

            throw std::invalid_argument("key not found in settings map");
        }

        inline void SetSettingValue(const std::string& name, std::any value)
        {
            _settings[name] = value;
        }

    protected:
        std::unordered_map<std::string, std::any> _settings;

        std::filesystem::path _settingFile;

    private:
        void loadSettings(const std::filesystem::path& path);

        std::string expandHomeDirectory(const std::string& path);
    };
} //namespace FlashBackClient
