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
            if (argv[i] == "--generate-configs")
            {
                FlashBackClient::ConfigManager::GenerateConfigs();
                return 0;
            }
        }
    }

    // Provide ConfigManager as SettingManager
    FlashBackClient::ServiceLocator::Provide<FlashBackClient::SettingManager>(
        new FlashBackClient::ConfigManager());

    if (!FlashBackClient::ServiceLocator::Get<FlashBackClient::SettingManager>()->Initialize())
    {
        std::cerr << "Failed to initialize config manager" << std::endl;
        return 1;
    }
    
    // Provide Scheduler as RuleManager
    FlashBackClient::ServiceLocator::Provide<FlashBackClient::RuleManager>(
        new FlashBackClient::Scheduler());
    
    if (!FlashBackClient::ServiceLocator::Get<FlashBackClient::RuleManager>()->Initialize())
    {
        std::cerr << "Failed to initialize scheduler" << std::endl;
        return 1;
    }
    
    auto defaultRules = FlashBackClient::ServiceLocator::Get<FlashBackClient::RuleManager>()->GetRules();

    return 0;
}