#include <flashbackclient/configs.h>

#include <stdexcept>

namespace FlashBackClient
{
    template <typename T>
    T ConfigManager::GetGlobalSetting(const std::string& settingName)
    {
        if (_globalSettings.find(settingName) != _globalSettings.end())
            return _globalSettings[settingName];

        throw std::invalid_argument("Setting not found");
    }

    void ConfigManager::loadConfigs()
    {
        // TODO: load ~/.config/flashbackclient/flashbackclient.yaml
    }
} //namespace FlashBackClient