/**
 * @file main.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Main entry point for the FlashBackClient program. Registers signal
 * handlers, checks command line arguments, and initializes and runs services
 *
 * @version 0.3
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
#include <flashbackclient/helper.h>
#include <flashbackclient/logging/logger.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/signal_handler.h>

#include <csignal>
#include <thread>

int main(int argc, char** argv)
{
    // Register signal handlers
    FlashBackClient::SignalHandler::Register();

    FlashBackClient::Logger::Initialize();

    FlashBackClient::ConfigManager::GenerateConfigs();

    if (!FlashBackClient::Helper::ProcessCommandLineArgs(argc, argv)) return 1;

    FlashBackClient::ServiceLocator::Provide<FlashBackClient::ConfigManager>(
        new FlashBackClient::ConfigManager());

    FlashBackClient::ServiceLocator::Provide<FlashBackClient::Scheduler>(
        new FlashBackClient::Scheduler());

    FlashBackClient::ServiceLocator::Get<FlashBackClient::Scheduler>()->Run();

    while (true) { std::this_thread::sleep_for(std::chrono::seconds(1)); }

    return 0;
}
