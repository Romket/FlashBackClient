#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace FlashBackClient
{
    class Logger
    {
    public:
        static void Initialize();
        static void SetVerbose();

        static void LogTrace();
        static void LogDebug();
        static void LogInfo();
        static void LogWarn();
        static void LogError();
        static void LogCritical();

    private:
        static std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _consoleSink;
        static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> _fileSink;
        static std::shared_ptr<spdlog::logger> _consoleLogger;
        static std::shared_ptr<spdlog::logger> _fileLogger;
    };


} // namespace FlashBackClient
