#include <flashbackclient/signal_handler.h>

#include <flashbackclient/configs.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>

#include "listener/platform_listener.h"

#include <iostream>

namespace FlashBackClient
{
    void SignalHandler::HandleShutdown(int signum)
    {
        std::cout << "Recieved shutdown signal: " << signum << std::endl;

        ServiceLocator::Shutdown<PlatformListener>();
        ServiceLocator::Shutdown<Scheduler>();
        ServiceLocator::Shutdown<ConfigManager>();

        exit(signum);
    }
} // namespace FlashBackClient
