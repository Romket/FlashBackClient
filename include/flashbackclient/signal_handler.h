#pragma once

namespace FlashBackClient
{
    class SignalHandler
    {
    public:
        static void HandleShutdown(int signum);
    };
} // namespace FlashBackClient
