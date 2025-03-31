/**
 * @file inotify_listener.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief InotifyListener implementation
 * @version 0.1
 * @date 2025-03-25
 *
 * @see inotify_listener.h
 * @see platform_listener.h
 * @sa listener.h
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "inotify_listener.h"

#include <filesystem>
#include <flashbackclient/defs.h>
#include <flashbackclient/listener.h>
#include <flashbackclient/logging/logger.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>

#include <cstring>
#include <sys/inotify.h>
#include <unistd.h>

#define flashback_ANY_FILE_EVENT                                              \
    (IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_CLOSE_WRITE | IN_MOVE_SELF | \
     IN_MOVED_FROM | IN_MOVED_TO)

namespace FlashBackClient
{
    bool InotifyListener::Initialize()
    {
        LOG_INFO("Initializing listener");

        LOG_TRACE("Initializing inotify file descriptor");
        _inotifyFd = inotify_init1(IN_NONBLOCK);
        if (_inotifyFd < 0)
        {
            LOG_ERROR("Failed to initialize inotify: {}", strerror(errno));
            return false;
        }

        LOG_TRACE("Initializing epoll file descriptor");
        _epollFd = epoll_create1(0);
        if (_epollFd < 0)
        {
            LOG_ERROR("Failed to initialize epoll: {}", strerror(errno));
            return false;
        }

        LOG_TRACE("Initializing self pipe");
        if (pipe2(_selfPipeFd, IN_CLOEXEC | IN_NONBLOCK) < 0)
        {
            LOG_ERROR("Failed to create self pipe: {}", strerror(errno));
            return false;
        }

        epoll_event event;
        event.events  = EPOLLIN;
        event.data.fd = _inotifyFd;

        LOG_TRACE("Adding inotify to epoll");
        if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _inotifyFd, &event) < 0)
        {
            LOG_ERROR("Failed to add inotify to epoll: {}", strerror(errno));
            return false;
        }

        LOG_TRACE("Adding self pipe to epoll");
        event.data.fd = _selfPipeFd[0];
        if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _selfPipeFd[0], &event) < 0)
        {
            LOG_ERROR("Failed to add self pipe to epoll: {}", strerror(errno));
            return false;
        }

        _running        = true;
        _listenerThread = std::thread([this]() { this->listenerThread(); });

        return true;
    }

    bool InotifyListener::Shutdown()
    {
        LOG_INFO("Shutting down listener");

        _running = false;

        if (_selfPipeFd[1] >= 0)
        {
            ssize_t result = write(_selfPipeFd[1], "0", 1);
            if (result == -1 && errno != EAGAIN)
            {
                LOG_ERROR("Failed to wake up listener thread: {}",
                          strerror(errno));
                return false;
            }
        }

        if (_listenerThread.joinable()) _listenerThread.join();

        safeClose(_selfPipeFd[0]);
        safeClose(_selfPipeFd[1]);
        safeClose(_epollFd);
        safeClose(_inotifyFd);

        return true;
    }

    bool InotifyListener::AddListener(ListenerCreateInfo& info, int depth)
    {
        bool isNew = true;
        for (auto& listener : _listeners)
        {
            if (listener.Path == info.path &&
                listener.BaseTarget == info.baseTarget)
            {
                listener.Conditions.push_back(info.owner);
                isNew = false;
            }
        }

        if (depth == 0)
            LOG_INFO("Adding listener in path: {}", info.path.string());

        if (depth > RECURSION_LIMIT) return false;

        if (depth > 0 && std::filesystem::is_directory(info.path))
        {
            LOG_TRACE("Reached file");
            return true;
        }

        if (info.baseTarget->IsIgnored(info.path))
        {
            LOG_INFO("Directory {} ignored", info.path.string());
            return true;
        }

        if (isNew)
        {
            int wd = inotify_add_watch(_inotifyFd, info.path.c_str(),
                                       flashback_ANY_FILE_EVENT);
            if (wd < 0)
            {
                LOG_INFO("Failed to create watch descripter: {}",
                         strerror(errno));
                return false;
            }

            _watchDescriptors[wd] = info.path.string();

            ListenerInfo newListener;
            newListener.Path = info.path;
            newListener.Conditions.push_back(info.owner);
            newListener.Status     = StatusEnum::active;
            newListener.LastUpdate = std::chrono::system_clock::now();
            newListener.BaseTarget = info.baseTarget;

            if (depth == 0) _listeners.push_back(newListener);
        }

        if (!std::filesystem::is_directory(info.path)) return true;

        // cppcheck-suppress useStlAlgorithm
        for (const auto& item : std::filesystem::directory_iterator(info.path))
        {
            if (item.is_directory())
            {
                ListenerCreateInfo subdirInfo;
                subdirInfo.path  = item.path();
                subdirInfo.owner = info.owner;

                if (!AddListener(subdirInfo, depth + 1)) return false;
            }
        }

        return true;
    }

    void InotifyListener::listenerThread()
    {
        while (_running)
        {
            int numEvents = epoll_wait(_epollFd, _events, MAX_EVENTS, -1);

            for (int i = 0; i < numEvents; i++)
            {
                if (_events[i].data.fd == _inotifyFd) processEvents();
                else if (_events[i].data.fd == _selfPipeFd[0])
                {
                    char buffer[1];
                    read(_selfPipeFd[0], buffer, 1);
                    break;
                }
            }
        }

        LOG_TRACE("Listener thread exiting");
    }

    void InotifyListener::processEvents()
    {
        char buffer[4096]
            __attribute__((aligned(__alignof__(struct inotify_event))));

        ssize_t length = read(_inotifyFd, buffer, sizeof(buffer));

        if (length == 0) return;

        if (length < 0)
        {
            if (errno != EAGAIN)
                LOG_ERROR("Error reading inotify events: {}", strerror(errno));
            return;
        }

        LOG_TRACE("Read {} bytes", length);

        ssize_t i = 0;
        while (i < length)
        {
            struct inotify_event* event =
                reinterpret_cast<struct inotify_event*>(&buffer[i]);

            if (event->mask & IN_Q_OVERFLOW || event->mask & IN_IGNORED)
            {
                LOG_WARN("Event overflow or ignored");
                i += sizeof(struct inotify_event) + event->len;
                continue;
            }

            std::string path =
                _watchDescriptors.find(event->wd)->second + "/" + event->name;

            LOG_INFO("Event: {}", event->mask);
            LOG_INFO("Path: {}", path);

            ListenerInfo info;
            info.Path       = path;
            info.LastUpdate = std::chrono::system_clock::now();

            if (event->mask & IN_DELETE_SELF || event->mask & IN_MOVE_SELF)
            {
                LOG_TRACE("Target self deleted or self moved");
                info.Status = StatusEnum::self_modified;
            }
            else
            {
                LOG_TRACE("Target modified");
                info.Status = StatusEnum::modified;
            }

            std::filesystem::path normalizedPath =
                std::filesystem::absolute(path);

            for (auto& listener : _listeners)
            {
                std::filesystem::path normalizedListenerPath =
                    std::filesystem::absolute(listener.Path);

                Logger::LOG_INFO("Checking listener: {}",
                                 normalizedListenerPath.string());
                LOG_INFO("Against: {}", normalizedPath.string());

                if (!normalizedPath.string().find(
                        normalizedListenerPath.string()))
                {
                    LOG_INFO("Matched");
                    // LOG_DEBUG("Owner: {}", listener.Owner);

                    listener.Status     = StatusEnum::modified;
                    listener.LastUpdate = std::chrono::system_clock::now();

                    // ServiceLocator::Get<Scheduler>()->Flag();
                    if (!listener.BaseTarget->IsIgnored(normalizedPath))
                    {
                        for (const auto& condition : listener.Conditions)
                        {
                            // TODO: Check in reverse order (add CheckReverse()
                            // to Condition)
                            condition->Check({Triggers::on_file_change});
                        }
                        listener.BaseTarget->CheckRules(
                            {Triggers::on_file_change});
                    }
                }
            }

            i += sizeof(struct inotify_event) + event->len;
        }
    }

    void InotifyListener::safeClose(int& fd)
    {
        LOG_TRACE("Safe closing file descriptor {}", fd);

        if (fd < 0) return;
        close(fd);
        fd = -1;
    }
} // namespace FlashBackClient
