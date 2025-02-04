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
    };
} // namespace FlashBackClient
