#include <flashbackclient/configs.h>

namespace FlashBackClient
{
    bool ConfigManager::Initialize() { return SettingManager::Initialize(); }
    bool ConfigManager::Shutdown() { return SettingManager::Shutdown(); }

    void ConfigManager::GenerateConfigs()
    {
        // TODO
    }
} // namespace FlashBackClient
