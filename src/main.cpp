#include <flashbackclient/service_locator.h>

#include "configs.h"
#include "scheduler.h"

#include <iostream>

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        for (int i = 0; i < argc; ++i)
        {
            if (std::string(argv[i]) == "--generate-configs")
            {
                FlashBackClient::ConfigManager::GenerateConfigs();
                return 0;
            }
        }
    }

    // Provide ConfigManager as SettingManager
    FlashBackClient::ServiceLocator::Provide<FlashBackClient::SettingManager>(
        new FlashBackClient::ConfigManager());

    // Provide Scheduler as RuleManager
    FlashBackClient::ServiceLocator::Provide<FlashBackClient::RuleManager>(
        new FlashBackClient::Scheduler());

    return 0;
}
