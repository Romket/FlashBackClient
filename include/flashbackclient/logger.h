#pragma once

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

namespace FlashBackClient
{
    class Logger
    {
    public:
        static void Initialize();
        static void SetVerbose();

    };
} // namespace FlashBackClient
