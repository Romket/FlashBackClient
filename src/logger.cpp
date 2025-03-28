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
        // Off by default so file is not written to
        _fileLogger->set_level(spdlog::level::off);
        _fileLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

        // Sets length of backtrace for fileLogger
        constexpr size_t backtraceLength = 32;
        _fileLogger->enable_backtrace(backtraceLength);

        _consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        _consoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] [%l] %v%$");

        _consoleLogger =
            std::make_shared<spdlog::logger>("_consoleLogger", _consoleSink);
        // _consoleLogger->set_level(spdlog::level::info);

        spdlog::register_logger(_fileLogger);
        spdlog::register_logger(_consoleLogger);

        LOG_INFO("Logger initialized");
    }

    void Logger::SetVerbose()
    {
        _consoleLogger->set_level(spdlog::level::trace);
        LOG_INFO("Verbose mode enabled");
    }

    void Logger::DumpFileLog()
    {
        LOG_INFO("Log dumped to {}/", LOG_DIR);
        _fileLogger->set_level(spdlog::level::trace);
        _fileLogger->dump_backtrace();
    }

    void Logger::Shutdown() { spdlog::shutdown(); }

} // namespace FlashBackClient
