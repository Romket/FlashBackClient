/**
 * @file defs.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Pre-compiler definitions for config paths, target paths, etc.
 * @version 0.1
 * @date 2025-03-25
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

#pragma once

#define HOME std::string(std::getenv("HOME"))

#define CONFIG_DIR       HOME + "/.config/flashbackclient"
#define CONFIG_FILE      "flashbackclient.yaml"
#define CONFIG_FILE_PATH CONFIG_DIR "/" CONFIG_FILE

#define SCHEDULER_CONFIG_DIR  HOME + "/.config/flashbackclient/scheduler"
#define SCHEDULER_CONFIG_FILE "scheduler.yaml"
#define SCHEDULER_CONFIG_FILE_PATH \
    SCHEDULER_CONFIG_DIR "/" SCHEDULER_CONFIG_FILE

#define TARGET_DEF_DIR HOME + "/.config/flashbackclient/targets"
#define EXAMPLE_TARGET_CONFIG_FILE "example.yaml"
#define EXAMPLE_TARGET_CONFIG_FILE_PATH \
    TARGET_DEF_DIR "/" EXAMPLE_TARGET_CONFIG_FILE

#define LOG_DIR                    HOME + "/.local/share/flashbackclient"
#define LOG_FILE_FMT_CRASH         "crash_%Y-%m-%d_%H-%M-%S.txt"
#define LOG_FILE_FMT_LOG           "log_%Y-%m-%d_%H-%M-%S.txt"
#define FILE_LOGGER_FORMAT         "[%Y-%m-%d %H:%M:%S.%e] [%l] %v"
#define CONSOLE_LOGGER_FORMAT_LOW  "%Y-%m-%d %H:%M:%S.%e [%^%l%$] %v"
#define CONSOLE_LOGGER_FORMAT_HIGH "%^%Y-%m-%d %H:%M:%S.%e [%l] %v%$"
#define BACKTRACE_LENGTH           32

#define RECURSION_LIMIT 10

#define THREAD_WAIT_TIME 1000
