#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
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
        FileChangeListener() = default;
        virtual ~FileChangeListener() = default;

        virtual bool Initialize() = 0;
        virtual bool Shutdown() = 0;

        virtual bool AddListener(const std::filesystem::path& path,
                                 ListenerType type = ListenerType::base) = 0;

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
            std::chrono::time_point LastUpdate = std::chrono::system_clock::now();
            StatusEnum Status = StatusEnum::inactive;
            ListenerType Type = ListenerType::base;
        };

        inline void listenerThread() { while (_running) processEvents(); }

        virtual void processEvents() = 0;

        std::atomic<bool> _running;

        // cppcheck-suppress unusedStructMember
        std::vector<ListenerInfo> _listeners;
    };
} //namespace FlashBackClient
