/**
 * @file signal_handler.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Implements signal handling and detection of errors versus expected
 * shutdowns
 *
 * @version 0.1
 * @date 2025-03-28
 *
 * @see signal_handler.h
 * @sa logger.cpp
 * @sa service_locator.h
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This file is part of FlashBackClient
 *
 * FlashBackClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FlashBackClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FlashBackClient.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <flashbackclient/signal_handler.h>

#include <flashbackclient/configs.h>
#include <flashbackclient/logging/logger.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>

#include "listener/platform_listener.h"

#include <csignal>
#include <cstdlib>

#if defined(_WIN32) || defined(_WIN64)
#include <dbghelp.h>
#include <windows.h>
#pragma comment(lib, "dbghelp.lib")
#else
#include <execinfo.h>
#endif

namespace FlashBackClient
{
    void SignalHandler::Register()
    {
        std::signal(SIGABRT, SignalHandler::handle);
        std::signal(SIGFPE, SignalHandler::handle);
        std::signal(SIGILL, SignalHandler::handle);
        std::signal(SIGINT, SignalHandler::handle);
        std::signal(SIGSEGV, SignalHandler::handle);
        std::signal(SIGTERM, SignalHandler::handle);

#ifndef _WIN32
        std::signal(SIGBUS, SignalHandler::handle);
        std::signal(SIGPIPE, SignalHandler::handle);
        std::signal(SIGQUIT, SignalHandler::handle);
        std::signal(SIGALRM, SignalHandler::handle);
        std::signal(SIGHUP, SignalHandler::handle);
#endif
    }

    void SignalHandler::handle(int signum)
    {
        if (isError(signum))
        {
            LOG_CRITICAL("Critical error detected: {} ({})", signum,
                         getSignalString(signum));
            logStackTrace();
        }
        else
        {
            LOG_INFO("Signal detected: {} ({})", signum,
                     getSignalString(signum));
        }

        LOG_TRACE("Reregistering signal handlers");
        Register();

        ServiceLocator::Shutdown<PlatformListener>();
        ServiceLocator::Shutdown<Scheduler>();
        ServiceLocator::Shutdown<ConfigManager>();

        LOG_INFO("Exiting with code {}", isError(signum) ? signum : 0);

        Logger::Shutdown(isError(signum));

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
        LOG_CRITICAL("Stack trace:");
        for (USHORT i = 0; i < frames; i++)
        {
            LOG_CRITICAL("  Frame {}: {}", i, stack[i]);
        }
#else
        void*  buffer[50];
        int    size    = backtrace(buffer, 50);
        char** symbols = backtrace_symbols(buffer, size);

        LOG_CRITICAL("Stack trace:");
        for (int i = 0; i < size; i++) { LOG_CRITICAL("  {}", symbols[i]); }
        free(symbols);
#endif
    }
} // namespace FlashBackClient
