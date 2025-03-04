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
