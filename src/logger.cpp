/**
 * @file logger.cpp
 * @author Chase Attebury (Appleberry) (chaseappleberryboi@gmail.com)
 * @brief Implements a custom logger including a custom logger and a file that
 * is dumped to on crash
 *
 * @version 0.1
 * @date 2025-03-28
 *
 * @see logger.h
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

#include <flashbackclient/logger.h>

#include <flashbackclient/defs.h>
#include <spdlog/async.h>
#include <spdlog/spdlog.h>

namespace FlashBackClient
{
    std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> Logger::_consoleSink;
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt>   Logger::_fileSink;
    std::shared_ptr<spdlog::logger>                      Logger::_consoleLogger;
    std::shared_ptr<spdlog::logger>                      Logger::_fileLogger;

    void Logger::Initialize()
    {
        constexpr size_t queueSize   = 8192; // Queue size for async logging
        constexpr size_t threadCount = 1;    // One logging thread
        spdlog::init_thread_pool(queueSize, threadCount);

        _fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            LOG_DIR + '/' + LOG_FILE_FMT, true);

        _fileLogger =
            std::make_shared<spdlog::logger>("_fileLogger", _fileSink);
        _fileLogger->set_level(spdlog::level::trace);
        _fileLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

        constexpr size_t backtraceLength = 32;
        _fileLogger->enable_backtrace(backtraceLength);

        _consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        _consoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] %v%$");

        _consoleLogger =
            std::make_shared<spdlog::logger>("_consoleLogger", _consoleSink);
        _consoleLogger->set_level(spdlog::level::info);

        spdlog::register_logger(_fileLogger);
        spdlog::register_logger(_consoleLogger);

        LOG_TRACE("Logger initialized");
    }

    void Logger::SetVerbose()
    {
        _consoleLogger->set_level(spdlog::level::trace);
        _consoleSink->set_level(spdlog::level::trace);
        LOG_TRACE("Verbose mode enabled, you should see this");
    }

    void Logger::DumpFileLog()
    {
        LOG_INFO("Log dumped to {}/", LOG_DIR);
        _fileLogger->dump_backtrace();
    }

    void Logger::Shutdown() { spdlog::shutdown(); }

} // namespace FlashBackClient
