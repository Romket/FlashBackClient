/**
 * @file helper.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Miscellaneous helper functions
 * @version 0.1
 * @date 2025-04-01
 *
 * @see helper.h
 * @sa main.cpp
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

#include <flashbackclient/helper.h>

#include <flashbackclient/configs.h>
#include <string>

namespace FlashBackClient
{
    bool Helper::ProcessCommandLineArgs(int argc, char** argv)
    {
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
                        FlashBackClient::LOG_WARN(
                            "Unknown log level inputted, defaulting to info");
                    }
                }
                else
                {
                    FlashBackClient::LOG_WARN("End of arguments reached, "
                                              "defaulting to info");
                }
            }
            else
            {
                FlashBackClient::LOG_ERROR("Unknown command line option {}",
                                           argv[i]);
                return false;
            }
        }

        return true;
    }
} // namespace FlashBackClient
