#include <cstdio>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <flashbackclient/defs.h>
#include <flashbackclient/logging/logger.h>

#include <spdlog/async.h>
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

namespace FlashBackClient
{
    std::shared_ptr<DualLevelSink>                       Logger::_consoleSink;
    std::shared_ptr<spdlog::sinks::basic_file_sink_mt>   Logger::_fileSink;
    std::shared_ptr<spdlog::logger>                      Logger::_consoleLogger;
    std::shared_ptr<spdlog::logger>                      Logger::_fileLogger;
    std::string Logger::_crashFilePath;
    bool Logger::_dumped;

    void Logger::Initialize()
    {
        _dumped = false;

        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "crash_%Y-%m-%d_%H-%M-%S.txt");

        _crashFilePath = LOG_DIR + '/' + oss.str();

        _fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
            _crashFilePath, true);

        _fileLogger =
            std::make_shared<spdlog::logger>("_fileLogger", _fileSink);
        
        // Off by default so file is not written to
        _fileLogger->set_level(spdlog::level::off);
        _fileLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

        // Sets length of backtrace for fileLogger
        constexpr size_t backtraceLength = 32;
        _fileLogger->enable_backtrace(backtraceLength);

        _consoleSink = std::make_shared<DualLevelSink>();

        _consoleLogger =
            std::make_shared<spdlog::logger>("_consoleLogger", _consoleSink);
        
        spdlog::register_logger(_consoleLogger);
        spdlog::register_logger(_fileLogger);

        LOG_INFO("Logger initialized");
    }

    void Logger::SetLogLevel(int level)
    {
        switch (level)
        {
        case 0:
            _consoleLogger->set_level(spdlog::level::trace);
            LOG_INFO("Logger set to level trace");
            return;
        case 1:
            _consoleLogger->set_level(spdlog::level::debug);
            LOG_INFO("Logger set to level debug");
            return;
        case 2:
            _consoleLogger->set_level(spdlog::level::info);
            LOG_INFO("Logger set to level info");
            return;
        case 3:
            _consoleLogger->set_level(spdlog::level::warn);
            return;
        case 4:
            _consoleLogger->set_level(spdlog::level::err);
            return;
        case 5:
            _consoleLogger->set_level(spdlog::level::critical);
            return;
        case 6:
            _consoleLogger->set_level(spdlog::level::off);
            return;
        default:
            _consoleLogger->set_level(spdlog::level::info);
            return;
        }
    }

    void Logger::DumpFileLog()
    {
        LOG_INFO("Log dumped to {}/", LOG_DIR);
        _fileLogger->set_level(spdlog::level::trace);
        _fileLogger->dump_backtrace();
        _dumped = true;
    }

    void Logger::Shutdown()
    {
        if (!_dumped)
        {
            if (remove(_crashFilePath.c_str()) == 0) 
            {
                LOG_INFO("Removed unwanted file log");
            } else 
            {
                LOG_ERROR("Failed to remove file log");
            }
        }
        spdlog::shutdown();
    }

} // namespace FlashBackClient
