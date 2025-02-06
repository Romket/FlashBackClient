#pragma once

#include <iostream>
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
            instance.reset();
        }

        template<typename T>
        static bool IsProvided()
        {
            if (getServiceInstance<T>()) return true;
            return false;
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
