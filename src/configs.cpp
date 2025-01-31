#include <flashbackclient/configs.h>

#include <filesystem>

namespace FlashBackClient
{
    bool ConfigManager::Initialize()
    {
        return SettingManager::Initialize();
    }

    void ConfigManager::GenerateConfigs()
    {
        // TODO
    }
} //namespace FlashBackClient
