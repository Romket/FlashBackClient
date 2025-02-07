#include <cstdlib>
#include <flashbackclient/signal_handler.h>

#include <flashbackclient/configs.h>
#include <flashbackclient/logger.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>

#include "listener/platform_listener.h"

#include <csignal>

#if defined(_WIN32) || defined(_WIN64)
#include <dbghelp.h>
#include <windows.h>
#pragma comment(lib, "dbghelp.lib")
#else
#include <execinfo.h>
#endif

namespace FlashBackClient
{
    void SignalHandler::Handle(int signum)
    {
        if (isError(signum))
        {
            Logger::LOG_CRITICAL("Critical error detected: {} ({})", signum,
                                 getSignalString(signum));
            logStackTrace();
        }
        else
        {
            Logger::LOG_INFO("Signal detected: {} ({})", signum,
                             getSignalString(signum));
        }

        ServiceLocator::Shutdown<PlatformListener>();
        ServiceLocator::Shutdown<Scheduler>();
        ServiceLocator::Shutdown<ConfigManager>();

        Logger::LOG_INFO("Exiting with code {}", isError(signum) ? signum : 0);

        exit(isError(signum) ? signum : 0);
    }

    const char* SignalHandler::getSignalString(int signum)
    {
        switch (signum)
        {
            case SIGABRT: return "SIGABRT";
            case SIGFPE: return "SIGFPE";
            case SIGILL: return "SIGILL";
            case SIGINT: return "SIGINT";
            case SIGSEGV: return "SIGSEGV";
            case SIGTERM: return "SIGTERM";
#ifndef _WIN32
            case SIGBUS: return "SIGBUS";
            case SIGPIPE: return "SIGPIPE";
            case SIGQUIT: return "SIGQUIT";
            case SIGALRM: return "SIGALRM";
            case SIGHUP: return "SIGHUP";
            case SIGUSR1: return "SIGUSR1";
            case SIGUSR2: return "SIGUSR2";
#endif
            default: return "Unknown signal";
        }
    }

    bool SignalHandler::isError(int signum)
    {
        switch (signum)
        {
            case SIGSEGV:
            case SIGFPE:
            case SIGILL:
            case SIGABRT:
#ifndef _WIN32
            case SIGBUS:
            case SIGPIPE:
#endif
                return true;
            default: return false;
        }
    }

    void SignalHandler::logStackTrace()
    {
#if defined(_WIN32) || defined(_WIN64)
        void*  stack[62];
        USHORT frames = CaptureStackBackTrace(0, 62, stack, NULL);
        Logger::LOG_CRITICAL("Stack trace:");
        for (USHORT i = 0; i < frames; i++)
        {
            Logger::LOG_CRITICAL("  Frame {}: {}", i, stack[i]);
        }
#else
        void*  buffer[50];
        int    size    = backtrace(buffer, 50);
        char** symbols = backtrace_symbols(buffer, size);

        Logger::LOG_CRITICAL("Stack trace:");
        for (int i = 0; i < size; i++)
        {
            Logger::LOG_CRITICAL("  {}", symbols[i]);
        }
        free(symbols);
#endif
    }
} // namespace FlashBackClient
