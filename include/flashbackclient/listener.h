#pragma once

#include <flashbackclient/defs.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <thread>
#include <vector>

namespace FlashBackClient
{
    enum class StatusEnum
    {
        inactive,
        active,
        // TODO: update target configs to include cases for self move, self
        // delete, etc.
        self_modified,
        modified
    };

    struct ListenerInfo
    {
        std::filesystem::path                              Path;
        std::chrono::time_point<std::chrono::system_clock> LastUpdate;
        StatusEnum Status = StatusEnum::inactive;
    };

    class FileChangeListener
    {
    public:
        FileChangeListener() : _running(false) {}
        virtual ~FileChangeListener() = default;

        virtual bool Initialize() = 0;
        virtual bool Shutdown()   = 0;

        virtual bool AddListener(const std::filesystem::path& path,
                                 int                          depth = 0) = 0;

        const std::vector<ListenerInfo>& GetListeners() const
        {
            return _listeners;
        }

        bool ListenerExists(const std::filesystem::path& path)
        {
            // TODO: std::any_of
            // cppcheck-suppress useStlAlgorithm
            for (const auto listener : _listeners)
            {
                if (listener.Path == path) return true;
            }

            return false;
        }

        void SetListenerStatus(const std::filesystem::path& path,
                               StatusEnum                   status)
        {
            for (auto& listener : _listeners)
            {
                if (listener.Path == path) { listener.Status = status; }
            }
        }

    protected:
        virtual void processEvents() = 0;

        std::atomic<bool>       _running;
        std::condition_variable _cv;
        std::mutex              _mutex;
        std::thread             _listenerThread;

        // cppcheck-suppress unusedStructMember
        std::vector<ListenerInfo> _listeners;

        virtual void listenerThread() = 0;
    };
} // namespace FlashBackClient
