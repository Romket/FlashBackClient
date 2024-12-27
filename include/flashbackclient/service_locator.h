#pragma once

#include <flashbackclient/configs.h>

#include <memory>

namespace FlashBackClient
{
    class ServiceLocator
    {
    public:
        // TODO: remember how to build a service locator
        /*inline std::unique_pointer<ConfigManager> GetConfigManager()
        {
            return _configManager;
        }

        inline void Provide(ConfigManager* manager)
        {
            _configManager =
        }*/

    private:
        std::unique_pointer<ConfigManager> _configManager;
    };
} //namespace FlashBackClient
