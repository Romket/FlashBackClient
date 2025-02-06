#include "inotify_listener.h"

#include <flashbackclient/defs.h>
#include <flashbackclient/listener.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>

#include <cstring>
#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>

#define flashback_ANY_FILE_EVENT                                              \
    (IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_CLOSE_WRITE | IN_MOVE_SELF | \
     IN_MOVED_FROM | IN_MOVED_TO)

namespace FlashBackClient
{
    bool InotifyListener::Initialize()
    {
        _inotifyFd = inotify_init1(IN_NONBLOCK);
        if (_inotifyFd < 0)
        {
            std::cerr << "Failed to initialize inotify: " << strerror(errno)
                      << std::endl;
            return false;
        }

        _epollFd = epoll_create1(0);
        if (_epollFd < 0)
        {
            std::cerr << "Failed to initialize epoll: " << strerror(errno)
                      << std::endl;
            return false;
        }

        if (pipe2(_selfPipeFd, IN_CLOEXEC | IN_NONBLOCK) < 0)
        {
            std::cerr << "Failed to create self pipe: " << strerror(errno)
                      << std::endl;
            return false;
        }

        epoll_event event;
        event.events  = EPOLLIN;
        event.data.fd = _inotifyFd;

        if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _inotifyFd, &event) < 0)
        {
            std::cerr << "Failed to add inotify to epoll: " << strerror(errno)
                      << std::endl;
            return false;
        }

        event.data.fd = _selfPipeFd[0];
        if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _selfPipeFd[0], &event) < 0)
        {
            std::cerr << "Failed to add self pipe to epoll: " << strerror(errno)
                      << std::endl;
            return false;
        }

        _running        = true;
        _listenerThread = std::thread([this]() { this->listenerThread(); });

        return true;
    }

    bool InotifyListener::Shutdown()
    {
        if (_selfPipeFd[1] >= 0)
        {
            ssize_t result = write(_selfPipeFd[1], "0", 1);
            if (result == -1 && errno != EAGAIN)
            {
                std::cerr << "Failed to wake up listener thread: "
                          << strerror(errno) << std::endl;
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

    bool InotifyListener::AddListener(ListenerInfo& info, int depth)
    {
        if (depth == 0)
            std::cout << "Adding listener in path " << info.Path << std::endl;

        if (depth > RECURSION_LIMIT) return false;

        int wd = inotify_add_watch(_inotifyFd, info.Path.c_str(),
                                   flashback_ANY_FILE_EVENT);
        if (wd < 0)
        {
            perror("Failed to create watch descriptor");
            return false;
        }

        _watchDescriptors[wd] = info.Path.string();

        info.Status     = StatusEnum::active;
        info.LastUpdate = std::chrono::system_clock::now();

        if (depth == 0) _listeners.push_back(info);

        if (!std::filesystem::is_directory(info.Path)) return true;

        // cppcheck-suppress useStlAlgorithm
        for (const auto& item : std::filesystem::directory_iterator(info.Path))
        {
            if (item.is_directory())
            {
                ListenerInfo subdirInfo;
                subdirInfo.Path = item.path();

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
                std::cerr << "Error reading inotify events: " << strerror(errno)
                          << std::endl;
            return;
        }

        std::cout << "Read " << length << " bytes" << std::endl;

        size_t i = 0;
        while (i < length)
        {
            struct inotify_event* event =
                reinterpret_cast<struct inotify_event*>(&buffer[i]);

            if (event->mask & IN_Q_OVERFLOW || event->mask & IN_IGNORED)
            {
                std::cout << "Event overflow or ignored" << std::endl;
                i += sizeof(struct inotify_event) + event->len;
                continue;
            }

            std::string path =
                _watchDescriptors.find(event->wd)->second + "/" + event->name;

            std::cout << "Event: " << event->mask << std::endl;
            std::cout << "Path: " << path << std::endl;

            ListenerInfo info;
            info.Path       = path;
            info.LastUpdate = std::chrono::system_clock::now();

            if (event->mask & IN_DELETE_SELF || event->mask & IN_MOVE_SELF)
                info.Status = StatusEnum::self_modified;
            else
                info.Status = StatusEnum::modified;

            std::filesystem::path normalizedPath =
                std::filesystem::absolute(path);

            for (auto& listener : _listeners)
            {
                std::filesystem::path normalizedListenerPath =
                    std::filesystem::absolute(listener.Path);

                std::cout << "Checking listener: " << normalizedListenerPath
                          << std::endl;
                std::cout << "Against: " << normalizedPath << std::endl;

                if (!normalizedPath.string().find(
                        normalizedListenerPath.string()))
                {
                    std::cout << "Matched" << std::endl;
                    listener.Status     = StatusEnum::modified;
                    listener.LastUpdate = std::chrono::system_clock::now();

                    // ServiceLocator::Get<Scheduler>()->Flag();
                    listener.Owner->CheckRules({Triggers::on_file_change});
                }
            }

            i += sizeof(struct inotify_event) + event->len;
        }
    }

    void InotifyListener::safeClose(int& fd)
    {
        if (fd < 0) return;
        close(fd);
        fd = -1;
    }
} // namespace FlashBackClient
