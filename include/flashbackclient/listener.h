#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <thread>
#include <vector>

namespace FlashBackClient
{
    class FileChangeListener
    {
    protected:
        enum class ListenerType
        {
            base,
            subdir
        };

    public:
        FileChangeListener() : _running(false) {}
        virtual ~FileChangeListener() = default;

        virtual bool Initialize() = 0;
        virtual bool Shutdown() = 0;

        virtual bool AddListener(const std::filesystem::path& path, int depth = 0) = 0;

    protected:
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
            ListenerType Type = ListenerType::base;
        };

        inline void listenerThread() { while (_running) processEvents(); }

        virtual void processEvents() = 0;

        std::atomic<bool> _running;
        std::thread _listenerThread;

        // cppcheck-suppress unusedStructMember
        std::vector<ListenerInfo> _listeners;
    };
} //namespace FlashBackClient
