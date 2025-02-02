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
        static void const inline OutputTrace();
        static void const inline OutputDebug();
        static void const inline OutputInfo();
        static void const inline OutputWarn();
        static void const inline OutputError();
        static void const inline OutputCritical();

    };
} // namespace FlashBackClient
