#include <flashbackclient/service_locator.h>

#include <flashbackclient/configs.h>
#include <flashbackclient/scheduler.h>

#include <flashbackclient/logger.h>

#include <iostream>

int main(int argc, char** argv)
{

    FlashBackClient::Logger::Init();

    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--generate-configs")
        {
            FlashBackClient::ConfigManager::GenerateConfigs();
            return 0;
        }
        else if (std::string(argv[i]) == "--verbose")
        {
            FlashBackClient::Logger::EnableVerboseMode();  // Enable verbose logging
        }
    }

    auto logger = FlashBackClient::Logger::GetLogger();
    logger->info("Application started");

    FlashBackClient::ServiceLocator::Provide<FlashBackClient::ConfigManager>(
        new FlashBackClient::ConfigManager());

    FlashBackClient::ServiceLocator::Provide<FlashBackClient::Scheduler>(
        new FlashBackClient::Scheduler());

    return 0;
}
