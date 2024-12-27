#pragma once

#include <string>
#include <unordered_map>

namespace FlashBackClient
{
    class ConfigManager
    {
    public:
        inline ConfigManager() : loadconfigs() {}

        inline std::unordered_map<std::string, any> GetGlobalSettings() { return _globalSettings; }
        
        template<typename T>
        T GetGlobalSetting(const std::string& settingName);

        void SetGlobalSetting(const std::string& settingName, any value);

    private:
        void loadconfigs();

        // cppcheck-suppress unusedStructMember
        std::unordered_map<std::string, any> _globalSettings;
    };
} //namespace FlashBackClient
