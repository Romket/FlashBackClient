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
        // TODO: update target configs to include cases for self move, self delete, etc.
        self_modified,
        modified
    };

    struct ListenerInfo
    {
        std::filesystem::path Path;
        std::chrono::time_point<std::chrono::system_clock> LastUpdate;
        StatusEnum Status = StatusEnum::inactive;
    };

    class FileChangeListener
    {
    public:
        FileChangeListener() : _running(false) {}
        virtual ~FileChangeListener() = default;

        virtual bool Initialize() = 0;
        virtual bool Shutdown() = 0;

        virtual bool AddListener(const std::filesystem::path& path, int depth = 0) = 0;

        // cppcheck-suppress returnByReference
        std::vector<ListenerInfo> GetBaseListeners() { return _baseListeners; }

    protected:
        virtual void processEvents() = 0;

        std::atomic<bool> _running;
        std::condition_variable _cv;
        std::mutex _mutex;
        std::thread _listenerThread;

        // cppcheck-suppress unusedStructMember
        std::vector<ListenerInfo> _baseListeners;
        // cppcheck-suppress unusedStructMember
        std::vector<ListenerInfo> _subListeners;

        inline void listenerThread()
        {
            while (_running)
            {
                {
                    std::unique_lock<std::mutex> lock(_mutex);
                    _cv.wait_for(lock, std::chrono::milliseconds(THREAD_WAIT_TIME),
                                 [this] { return !_running; });
                }

                if (!_running) break;

                processEvents();
            }
        }
    };
} //namespace FlashBackClient
