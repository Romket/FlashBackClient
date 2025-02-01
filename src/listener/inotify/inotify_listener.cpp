#include "inotify_listener.h"

#include <flashbackclient/defs.h>

#include <cstring>
#include <iostream>
#include <sys/inotify.h>
#include <unistd.h>

#define flashback_ANY_FILE_EVENT                                         \
    (IN_CREATE | IN_DELETE | IN_DELETE_SELF | IN_MODIFY | IN_MOVE_SELF | \
     IN_MOVED_FROM | IN_MOVED_TO)

namespace FlashBackClient
{
    bool InotifyListener::Initialize()
    {
        _inotifyFd = inotify_init1(IN_NONBLOCK);
        if (_inotifyFd < 0) return false;

        _running        = true;
        _listenerThread = std::thread([this]() { this->listenerThread(); });

        return true;
    }

    bool InotifyListener::Shutdown()
    {
        if (_inotifyFd >= 0)
        {
            close(_inotifyFd);
            _inotifyFd = -1;
        }

        return true;
    }

    bool InotifyListener::AddListener(const std::filesystem::path& path,
                                      int                          depth)
    {
        if (depth == 0)
            std::cout << "Adding listener in path " << path << std::endl;

        if (depth > RECURSION_LIMIT) return false;

        int wd = inotify_add_watch(_inotifyFd, path.c_str(),
                                   flashback_ANY_FILE_EVENT);
        if (wd < 0)
        {
            perror("Failed to create watch descriptor");
            return false;
        }

        _watchDescriptors[wd] = path.string();

        if (depth == 0)
        {
            ListenerInfo info;
            info.Path       = path;
            info.Status     = StatusEnum::active;
            info.LastUpdate = std::chrono::system_clock::now();
            _listeners.push_back(info);
        }

        if (!std::filesystem::is_directory(path)) return true;

        // cppcheck-suppress useStlAlgorithm
        for (const auto& item : std::filesystem::directory_iterator(path))
        {
            if (item.is_directory())
            {
                if (!AddListener(item.path(), depth + 1)) return false;
            }
        }

        return true;
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
                }
            }

            i += sizeof(struct inotify_event) + event->len;
        }
    }
} // namespace FlashBackClient
