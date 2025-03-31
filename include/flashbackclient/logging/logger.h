#pragma once

#include <cstring>
#include <filesystem>
#include <memory>

#include <flashbackclient/logging/dualsink.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace FlashBackClient
{
    class Logger
    {

    private:
        static std::shared_ptr<DualLevelSink> _consoleSink;
        static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> _fileSink;
        static std::shared_ptr<spdlog::logger> _consoleLogger;
        static std::shared_ptr<spdlog::logger> _fileLogger;
        static std::string _crashFilePath;
        static bool _dumped;

    public:
        static void Initialize();
        static void SetLogLevel(int level);
        static void DumpFileLog();
        static void Shutdown();

        // Don't question the std::forward<Args>(args)...
        // Cause I don't know either
        template<typename... Args>
        static void logTrace(const char* file, int line, const char* func,
                             fmt::format_string<Args...> fmtStr, Args&&... args)
        {
            SPDLOG_LOGGER_TRACE(
                _consoleLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
            SPDLOG_LOGGER_TRACE(
                _fileLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void logDebug(const char* file, int line, const char* func,
                             fmt::format_string<Args...> fmtStr, Args&&... args)
        {
            SPDLOG_LOGGER_DEBUG(
                _consoleLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
            SPDLOG_LOGGER_DEBUG(
                _fileLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void logInfo(const char* file, int line, const char* func,
                            fmt::format_string<Args...> fmtStr, Args&&... args)
        {
            SPDLOG_LOGGER_INFO(
                _consoleLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
            SPDLOG_LOGGER_INFO(
                _fileLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void logWarn(const char* file, int line, const char* func,
                            fmt::format_string<Args...> fmtStr, Args&&... args)
        {
            SPDLOG_LOGGER_WARN(
                _consoleLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
            SPDLOG_LOGGER_WARN(
                _fileLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void logError(const char* file, int line, const char* func,
                             fmt::format_string<Args...> fmtStr, Args&&... args)
        {
            SPDLOG_LOGGER_ERROR(
                _consoleLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
            SPDLOG_LOGGER_ERROR(
                _fileLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void logCritical(const char* file, int line, const char* func,
                                fmt::format_string<Args...> fmtStr,
                                Args&&... args)
        {
            SPDLOG_LOGGER_CRITICAL(
                _consoleLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
            SPDLOG_LOGGER_CRITICAL(
                _fileLogger, "{}:{} [{}] - {}", file, line, func,
                fmt::format(fmtStr, std::forward<Args>(args)...));
        }
    };

} // namespace FlashBackClient

#define LOG_TRACE(fmt, ...)                                                    \
    Logger::logTrace(                                                          \
        std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__, \
        __func__, fmt, ##__VA_ARGS__)

#define LOG_DEBUG(fmt, ...)                                                    \
    Logger::logDebug(                                                          \
        std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__, \
        __func__, fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...)                                                     \
    Logger::logInfo(                                                           \
        std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__, \
        __func__, fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...)                                                     \
    Logger::logWarn(                                                           \
        std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__, \
        __func__, fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...)                                                    \
    Logger::logError(                                                          \
        std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__, \
        __func__, fmt, ##__VA_ARGS__)

#define LOG_CRITICAL(fmt, ...)                                                 \
    Logger::logCritical(                                                       \
        std::filesystem::path(__FILE__).filename().string().c_str(), __LINE__, \
        __func__, fmt, ##__VA_ARGS__)
