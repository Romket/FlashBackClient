#pragma once

#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/defs.h>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace FlashBackClient
{
    class ConfigManager : public SettingManager
    {
    public:
        inline ConfigManager() : SettingManager(CONFIG_FILE_PATH) {}

        static void GenerateConfigs();
    };
} //namespace FlashBackClient
