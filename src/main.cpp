#include <flashbackclient/service_locator.h>

#include <flashbackclient/configs.h>
#include <flashbackclient/scheduler.h>

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

    FlashBackClient::ServiceLocator::Provide<FlashBackClient::ConfigManager>(
        new FlashBackClient::ConfigManager());

    FlashBackClient::ServiceLocator::Provide<FlashBackClient::Scheduler>(
        new FlashBackClient::Scheduler());

    return 0;
}
