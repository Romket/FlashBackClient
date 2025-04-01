/**
 * @file main.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Main entry point for the FlashBackClient program. Registers signal
 * handlers, checks command line arguments, and initializes and runs services
 *
 * @version 0.1
 * @date 2025-03-28
 *
 * @sa service_locator.h
 * @sa signal_handler.h
 * @sa signal_handler.cpp
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <flashbackclient/service_locator.h>

#include <flashbackclient/configs.h>
#include <flashbackclient/logging/logger.h>
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
        else if (std::string(argv[i]) == "--log-level")
        {
            if (++i < argc)
            {
                if (std::string(argv[i]) == "trace")
                {
                    FlashBackClient::Logger::SetLogLevel(0);
                }
                else if (std::string(argv[i]) == "debug")
                {
                    FlashBackClient::Logger::SetLogLevel(1);
                }
                else if (std::string(argv[i]) == "info")
                {
                    FlashBackClient::Logger::SetLogLevel(2);
                }
                else if (std::string(argv[i]) == "warn")
                {
                    FlashBackClient::Logger::SetLogLevel(3);
                }
                else if (std::string(argv[i]) == "error")
                {
                    FlashBackClient::Logger::SetLogLevel(4);
                }
                else if (std::string(argv[i]) == "critical")
                {
                    FlashBackClient::Logger::SetLogLevel(5);
                }
                else if (std::string(argv[i]) == "off")
                {
                    FlashBackClient::Logger::SetLogLevel(6);
                }
                else
                {
                    FlashBackClient::LOG_WARN("Unknown log level "
                                              "inputted, defaulting to info");
                }
            }
            else
            {
                FlashBackClient::LOG_WARN("End of arguments reached, "
                                          "defaulting to info");
            }
        }
        else if (std::string(argv[i]) == "--always-file-log")
        {
            FlashBackClient::Logger::AlwaysFileLog();
        }
        else if (std::string(argv[i]) == "--backtrace-length")
        {
            if (++i < argc)
            {
                try
                {
                    FlashBackClient::Logger::SetBacktraceLength(
                        std::stoi(argv[i]));
                }
                catch (const std::invalid_argument& e)
                {
                    FlashBackClient::LOG_ERROR("Not a valid integer.");
                }
                catch (const std::out_of_range& e)
                {
                    FlashBackClient::LOG_ERROR("Error: Number out of range.");
                }
            }
            else
            {
                FlashBackClient::LOG_WARN("End of arguments reached, "
                                          "backtrace length unchanged");
            }
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
