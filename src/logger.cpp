#include <flashbackclient/logger.h>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
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
} // namespace FlashBackClient
