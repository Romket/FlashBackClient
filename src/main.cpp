#include <flashbackclient/service_locator.h>

#include <flashbackclient/configs.h>
#include <flashbackclient/logger.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/signal_handler.h>

#include <csignal>
#include <thread>

int main(int argc, char** argv)
{
    // Register signal handlers
    signal(SIGHUP, FlashBackClient::SignalHandler::HandleShutdown);
    signal(SIGINT, FlashBackClient::SignalHandler::HandleShutdown);
    signal(SIGTERM, FlashBackClient::SignalHandler::HandleShutdown);

    FlashBackClient::Logger::Initialize();

    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--generate-configs")
        {
            FlashBackClient::ConfigManager::GenerateConfigs();
            return 0;
        }
        else if (std::string(argv[i]) == "--verbose")
        {
            FlashBackClient::Logger::SetVerbose();
        }
    }

    FlashBackClient::ServiceLocator::Provide<FlashBackClient::ConfigManager>(
        new FlashBackClient::ConfigManager());

    FlashBackClient::ServiceLocator::Provide<FlashBackClient::Scheduler>(
        new FlashBackClient::Scheduler());

    FlashBackClient::ServiceLocator::Get<FlashBackClient::Scheduler>()->Run();

    while (true) { std::this_thread::sleep_for(std::chrono::seconds(1)); }

    return 0;
}
