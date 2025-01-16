#include <flashbackclient/service_locator.h>

#include "configs.h"
#include "scheduler.h"

#include <iostream>

int main()
{
    // Provide ConfigManager as SettingManager
    FlashBackClient::ServiceLocator::Provide<FlashBackClient::SettingManager>(
        new FlashBackClient::ConfigManager());
    
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