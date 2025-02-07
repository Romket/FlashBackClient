#pragma once

namespace FlashBackClient
{
    class SignalHandler
    {
    public:
        static void Handle(int signum);

    private:
        static const char* getSignalString(int signum);
        static bool        isError(int signum);
        static void        logStackTrace();
    };
} // namespace FlashBackClient
