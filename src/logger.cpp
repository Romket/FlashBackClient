#include <flashbackclient/logger.h>

namespace FlashBackClient
{
    void Logger::Initialize()
    {
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_pattern("%^[%T] [%l] %v%$");

        auto console_logger = std::make_shared<spdlog::async_logger>(
            "console", console_sink, spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        
        spdlog::register_logger(console_logger);

        console_logger->set_level(spdlog::level::err);
        spdlog::get("console")->error("This is a test of formatting and scope #1");

    }

    void Logger::SetVerbose()
    {
        spdlog::get("console")->set_level(spdlog::level::trace);
        spdlog::get("console")->info("This is a test of formatting and scope #2");
    }

} // namespace FlashBackClient
