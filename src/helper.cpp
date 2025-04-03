/**
 * @file helper.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Miscellaneous helper functions
 * @version 0.2
 * @date 2025-04-01
 *
 * @see helper.h
 * @sa main.cpp
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This file is part of FlashBackClient
 *
 * FlashBackClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FlashBackClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FlashBackClient.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <flashbackclient/helper.h>

#include <flashbackclient/configs.h>
#include <flashbackclient/license_info.h>
#include <flashbackclient/logging/logger.h>
#include <iostream>
#include <string>

namespace FlashBackClient
{
    void Helper::DisplayNotice() { std::cout << NOTICE << std::endl; }

    ArgsResult Helper::ProcessCommandLineArgs(int argc, char** argv)
    {
        for (int i = 1; i < argc; ++i)
        {
            if (std::string(argv[i]) == "--generate-configs")
            {
                ConfigManager::GenerateConfigs();
                return ArgsResult::exit;
            }
            else if (std::string(argv[i]) == "--log-level")
            {
                if (++i < argc)
                {
                    if (std::string(argv[i]) == "trace")
                    {
                        Logger::SetLogLevel(0);
                    }
                    else if (std::string(argv[i]) == "debug")
                    {
                        Logger::SetLogLevel(1);
                    }
                    else if (std::string(argv[i]) == "info")
                    {
                        Logger::SetLogLevel(2);
                    }
                    else if (std::string(argv[i]) == "warn")
                    {
                        Logger::SetLogLevel(3);
                    }
                    else if (std::string(argv[i]) == "error")
                    {
                        Logger::SetLogLevel(4);
                    }
                    else if (std::string(argv[i]) == "critical")
                    {
                        Logger::SetLogLevel(5);
                    }
                    else if (std::string(argv[i]) == "off")
                    {
                        Logger::SetLogLevel(6);
                    }
                    else
                    {
                        LOG_WARN(
                            "Unknown log level inputted, defaulting to info");
                    }
                }
                else
                {
                    LOG_WARN("End of arguments reached, "
                             "defaulting to info");
                }
            }
            else if (std::string(argv[i]) == "--always-file-log")
            {
                Logger::AlwaysFileLog();
            }
            else if (std::string(argv[i]) == "--backtrace-length")
            {
                if (++i < argc)
                {
                    try
                    {
                        Logger::SetBacktraceLength(std::stoi(argv[i]));
                    }
                    catch (const std::invalid_argument& e)
                    {
                        LOG_ERROR("Not a valid integer.");
                    }
                    catch (const std::out_of_range& e)
                    {
                        LOG_ERROR("Error: Number out of range.");
                    }
                }
                else
                {
                    LOG_WARN(
                        "End of arguments reached, backtrace length unchanged");
                }
            }
            else if (std::string(argv[i]) == "--show")
            {
                if (++i < argc)
                {
                    if (std::string(argv[i]) == "w")
                    {
                        std::cout << SHOW_W << std::endl;
                    }
                    else if (std::string(argv[i]) == "c")
                    {
                        std::cout << SHOW_C << std::endl;
                    }
                    else
                    {
                        LOG_ERROR(
                            "Incorrect usage of --show: unknown option {}",
                            argv[i]);
                        return ArgsResult::err;
                    }
                }
                else
                {
                    LOG_ERROR("Incorrect usage of --show: no option given");
                    return ArgsResult::err;
                }
            }
            else
            {
                LOG_ERROR("Unknown command line option {}", argv[i]);
                return ArgsResult::err;
            }
        }

        return ArgsResult::cont;
    }
} // namespace FlashBackClient
