/**
 * @file service_locator.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Service locator implementation
 * @version 0.1
 * @date 2025-03-25
 *
 * @see configs.h
 * @see platform_listener.h
 * @see scheduler.h
 * @sa listener.h
 * @sa rulemanager.h
 * @sa settingmanager.h
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

#include <flashbackclient/logging/logger.h>
#include <memory>
#include <stdexcept>

namespace FlashBackClient
{
    class ServiceLocator
    {
    public:
        // Provide a service
        template<typename T>
        static void Provide(T* service)
        {
            LOG_TRACE("Providing service");
            // LOG_DEBUG("ptr: {}", service);

            if (!service)
            {
                throw std::invalid_argument("Cannot provide null service");
            }

            auto& instance = getServiceInstance<T>();
            if (instance != nullptr)
            {
                throw std::invalid_argument(
                    "Service already provided. Please call Shutdown<T> first.");
            }
            instance = std::unique_ptr<T>(service);

            if (!instance->Initialize())
            {
                throw std::invalid_argument("Failed to initialize service");
            }
        }

        // Get the provided service
        template<typename T>
        static T* Get()
        {
            auto& instance = getServiceInstance<T>();
            if (!instance)
            {
                throw std::runtime_error("Service not provided.");
            }
            return instance.get();
        }

        // Shutdown a service
        template<typename T>
        static void Shutdown()
        {
            auto& instance = getServiceInstance<T>();
            if (instance)
            {
                LOG_TRACE("Shutting down service");
                // LOG_DEBUG("ptr: {}", instance);

                instance->Shutdown();
                instance.reset();
            }
            else { LOG_WARN("Service already shut down or never provided"); }
        }

        template<typename T>
        static bool IsProvided()
        {
            return getServiceInstance<T>() != nullptr;
        }

    private:
        template<typename T>
        static std::unique_ptr<T>& getServiceInstance()
        {
            static std::unique_ptr<T> instance = nullptr;
            return instance;
        }
    };
} // namespace FlashBackClient
