#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace FlashBackClient
{
    class DualLevelSink : public spdlog::sinks::stdout_color_sink_mt
    {
    private:
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _traceSink;
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _warnSink;
    public:
        virtual ~DualLevelSink() = default;
        DualLevelSink();

        void Initialize();
        
        void log(const spdlog::details::log_msg& msg) override;
        void flush() override;
    };
} // namespace FlashBackClient