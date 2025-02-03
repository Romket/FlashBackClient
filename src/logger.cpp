#include <flashbackclient/logger.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

namespace FlashBackClient
{
    void Logger::Initialize()
    {

        spdlog::init_thread_pool(8192, 1);

        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("%^[%T] [%l] %v%$");

        auto console_logger = std::make_shared<spdlog::async_logger>(
            "console", console_sink, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        
        spdlog::register_logger(console_logger);

        console_logger->set_level(spdlog::level::err);

    }

    void Logger::SetVerbose()
    {
        spdlog::get("console")->set_level(spdlog::level::trace);
    }

    void Logger::OutputTrace(const std::string& message)
    {
        spdlog::get("console")->trace(message);
    }

    void Logger::OutputDebug(const std::string& message)
    {
        spdlog::get("console")->debug(message);
    }

    void Logger::OutputInfo(const std::string& message)
    {
        spdlog::get("console")->info(message);
    }

    void Logger::OutputWarn(const std::string& message)
    {
        spdlog::get("console")->warn(message);
    }

    void Logger::OutputError(const std::string& message)
    {
        spdlog::get("console")->error(message);
    }

    void Logger::OutputCritical(const std::string& message)
    {
        spdlog::get("console")->critical(message);
    }
} // namespace FlashBackClient
