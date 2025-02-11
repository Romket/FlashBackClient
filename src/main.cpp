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
    std::signal(SIGABRT, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGFPE, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGILL, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGINT, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGSEGV, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGTERM, FlashBackClient::SignalHandler::Handle);

#ifndef _WIN32
    std::signal(SIGBUS, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGPIPE, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGQUIT, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGALRM, FlashBackClient::SignalHandler::Handle);
    std::signal(SIGHUP, FlashBackClient::SignalHandler::Handle);
#endif

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
        else
        {
            FlashBackClient::LOG_ERROR("Unknown command line option {}",
                                       argv[i]);
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
