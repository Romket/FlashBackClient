#pragma once

#include <memory>
#include <string>

namespace FlashBackClient
{
    class Logger
    {
    public:
        static void Initialize();
        static void SetVerbose();

        static void OutputTrace(const std::string& message);
        static void OutputDebug(const std::string& message);
        static void OutputInfo(const std::string& message);
        static void OutputWarn(const std::string& message);
        static void OutputError(const std::string& message);
        static void OutputCritical(const std::string& message);
    };
} // namespace FlashBackClient
