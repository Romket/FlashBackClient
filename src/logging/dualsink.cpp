#include <flashbackclient/logging/dualsink.h>

namespace FlashBackClient
{
    DualLevelSink::DualLevelSink()
    {
        _traceSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        _traceSink->set_level(spdlog::level::trace);
        _traceSink->set_pattern("%Y-%m-%d %H:%M:%S.%e [%^%l%$] %v");

        _warnSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        _warnSink->set_level(spdlog::level::warn);
        _warnSink->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%l] %v%$");
    }

    void DualLevelSink::log(const spdlog::details::log_msg& msg)
    {
        if(msg.level >= spdlog::level::warn)
        {
            _warnSink->log(msg);
        }
        else 
        {
            _traceSink->log(msg);
        }
    }

    void DualLevelSink::flush()
    {
        _traceSink->flush();
        _warnSink->flush();
    }
} // namespace FlashBackClient