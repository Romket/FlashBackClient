#pragma once

#include <string>
#include <unordered_map>

namespace FlashBackClient
{
    struct setting
    {
        // cppcheck-suppress unusedStructMember
        std::string name;
        any value;
    };

    class ConfigManager
    {
    public:
        inline ConfigManager() : loadconfigs() {}

        std::unordered_map<setting> GetSettings();
        setting GetSetting(const std::string& settingName)

    private:
        void loadconfigs();

        // cppcheck-suppress unusedStructMember
        std::unordered_map<setting> settings;
    };
} //namespace FlashBackClient
