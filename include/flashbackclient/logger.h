#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace FlashBackClient
{
    class Logger
    {

    private:
        static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _consoleSink;
        static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> _fileSink;
        static std::shared_ptr<spdlog::logger> _consoleLogger;
        static std::shared_ptr<spdlog::logger> _fileLogger;

    public:
        static void Initialize();
        static void SetVerbose();

        // Don't question the std::forward<Args>(args)...
        // Cause I don't know either
        template <typename... Args>
        void logTrace(Args&&... args)
        {
            SPDLOG_LOGGER_TRACE(_consoleLogger, std::forward<Args>(args)...);
            SPDLOG_LOGGER_TRACE(_fileLogger, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void logDebug(Args&&... args)
        {
            SPDLOG_LOGGER_DEBUG(_consoleLogger, std::forward<Args>(args)...);
            SPDLOG_LOGGER_DEBUG(_fileLogger, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void logInfo(Args&&... args)
        {
            SPDLOG_LOGGER_INFO(_consoleLogger, std::forward<Args>(args)...);
            SPDLOG_LOGGER_INFO(_fileLogger, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void logWarn(Args&&... args)
        {
            SPDLOG_LOGGER_WARN(_consoleLogger, std::forward<Args>(args)...);
            SPDLOG_LOGGER_WARN(_fileLogger, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void logError(Args&&... args)
        {
            SPDLOG_LOGGER_ERROR(_consoleLogger, std::forward<Args>(args)...);
            SPDLOG_LOGGER_ERROR(_fileLogger, std::forward<Args>(args)...);
        }

        template <typename... Args>
        void logCritical(Args&&... args)
        {
            SPDLOG_LOGGER_CRITICAL(_consoleLogger, std::forward<Args>(args)...);
            SPDLOG_LOGGER_CRITICAL(_fileLogger, std::forward<Args>(args)...);
        }
    };


} // namespace FlashBackClient
