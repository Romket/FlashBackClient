#pragma once

#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/defs.h>

namespace FlashBackClient
{
    class ConfigManager : public SettingManager
    {
    public:
        inline ConfigManager() : SettingManager(CONFIG_FILE_PATH) {}

        bool Initialize() override;
        bool Shutdown() override;

        static void GenerateConfigs();
    };
} // namespace FlashBackClient
