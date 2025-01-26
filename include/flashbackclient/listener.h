#pragma once

#include <atomic>
#include <chrono>
#include <filesystem>
#include <vector>

namespace FlashBackClient
{
    class FileChangeListener
    {
    public:
        FileChangeListener() = default;
        virtual ~FileChangeListener() = default;

        virtual bool Initialize() = 0;
        virtual bool Shutdown() = 0;

        virtual bool AddListener(const std::filesystem::path& path) = 0;

    protected:
        struct ListenerInfo
        {
            std::filesystem::path Path;
            std::chrono::time_point LastUpdate = std::chrono::system_clock::now();
            bool Status = false;
            bool Active = false;
        };

        inline void listenerThread() { while (_running) processEvents(); }

        virtual void processEvents() = 0;

        std::atomic<bool> _running;

        // cppcheck-suppress unusedStructMember
        std::vector<ListenerInfo> _listeners;
    };
} //namespace FlashBackClient
