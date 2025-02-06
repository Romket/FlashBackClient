#pragma once

#include <flashbackclient/defs.h>
#include <flashbackclient/target.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <memory>
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
        std::chrono::time_point<std::chrono::system_clock> LastUpdate;
        std::shared_ptr<Target>                            Owner = nullptr;
        std::filesystem::path                              Path;
        StatusEnum Status = StatusEnum::inactive;
    };

    class FileChangeListener
    {
    public:
        FileChangeListener() : _running(false) {}
        virtual ~FileChangeListener() = default;

        virtual bool Initialize() = 0;
        virtual bool Shutdown()   = 0;

        virtual bool AddListener(ListenerInfo& info, int depth = 0) = 0;

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

        std::atomic<bool> _running;
        std::thread       _listenerThread;

        // cppcheck-suppress unusedStructMember
        std::vector<ListenerInfo> _listeners;

        virtual void listenerThread() = 0;
    };
} // namespace FlashBackClient
