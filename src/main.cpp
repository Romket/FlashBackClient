#include <flashbackclient/service_locator.h>

#include "configs.h"
#include "scheduler.h"

int main()
{
    // Provide ConfigManager as SettingManager
    FlashBackClient::ServiceLocator::Provide<FlashBackClient::SettingManager>(
        new FlashBackClient::ConfigManager());
    
    // Provide Scheduler as RuleManager
    FlashBackClient::ServiceLocator::Provide<FlashBackClient::RuleManager>(
        new FlashBackClient::Scheduler());

    // Retrieve and use services (example)
    auto configs = FlashBackClient::ServiceLocator::Get<FlashBackClient::SettingManager>();
    auto rules = FlashBackClient::ServiceLocator::Get<FlashBackClient::RuleManager>();

    // Shutdown services (example)
    FlashBackClient::ServiceLocator::Shutdown<FlashBackClient::SettingManager>();
    FlashBackClient::ServiceLocator::Shutdown<FlashBackClient::RuleManager>();

    return 0;
}