#pragma once

#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/defs.h>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace FlashBackClient
{
    class ConfigManager : private SettingManager
    {
    public:
        inline ConfigManager() : SettingManager(CONFIG_FILE_PATH) {}
    };
} //namespace FlashBackClient
