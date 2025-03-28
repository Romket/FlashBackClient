/**
 * @file listener.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines a virtual listener class for file change events
 * @version 0.1
 * @date 2025-03-25
 *
 * @see platform_listener.h
 * @sa inotify_listener.h
 * @sa inotify_listener.cpp
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

#pragma once

#include <flashbackclient/condition.h>
#include <flashbackclient/defs.h>
#include <flashbackclient/target.h>

#include <atomic>
#include <chrono>
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
        std::chrono::system_clock::time_point LastUpdate;
        // cppcheck-suppress unusedStructMember
        std::vector<Condition*> Conditions;
        std::filesystem::path   Path;
        StatusEnum              Status = StatusEnum::inactive;
        // cppcheck-suppress unusedStructMember
        Target* BaseTarget;
    };

    struct ListenerCreateInfo
    {
        std::filesystem::path path;
        // cppcheck-suppress unusedStructMember
        Condition* owner;
        // cppcheck-suppress unusedStructMember
        Target* baseTarget;
    };

    class FileChangeListener
    {
    public:
        FileChangeListener() : _running(false) {}
        virtual ~FileChangeListener() = default;

        virtual bool Initialize() = 0;
        virtual bool Shutdown()   = 0;

        virtual bool AddListener(ListenerCreateInfo& info, int depth = 0) = 0;

        const std::vector<ListenerInfo>& GetListeners() const
        {
            return _listeners;
        }

        bool ListenerExists(const std::filesystem::path& path)
        {
            // TODO: std::any_of
            // cppcheck-suppress useStlAlgorithm
            for (const auto& listener : _listeners)
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
