/**
 * @file inotify_listener.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines the unix implementation of the FileChangeListener class
 * @version 0.1
 * @date 2025-03-25
 *
 * @see inotify_listener.cpp
 * @see listener.h
 * @see platform_listener.h
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

#pragma once

#include <flashbackclient/listener.h>

#include <string>
#include <sys/epoll.h>
#include <unordered_map>

#define MAX_EVENTS 1024

namespace FlashBackClient
{
    class InotifyListener : public FileChangeListener
    {
    public:
        InotifyListener() :
            _inotifyFd(-1), _epollFd(-1), _selfPipeFd {-1, -1},
            FileChangeListener()
        {
        }

        virtual ~InotifyListener() = default;

        bool Initialize() override;
        bool Shutdown() override;

        bool AddListener(ListenerCreateInfo& info, int depth = 0) override;

    private:
        void listenerThread() override;

        void processEvents() override;

        void safeClose(int& fd);

        int _epollFd;
        int _inotifyFd;
        int _selfPipeFd[2];

        epoll_event _events[MAX_EVENTS];

        // cppcheck-suppress unusedStructMember
        std::unordered_map<int, std::string> _watchDescriptors;
    };
} // namespace FlashBackClient
