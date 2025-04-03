/**
 * @file settingmanager.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines the SettingManager class for loading and handling settings
 * @version 0.1
 * @date 2025-03-25
 *
 * @see settingmanager.cpp
 * @sa configs.h
 * @sa configs.cpp
 * @sa target.h
 * @sa target.cpp
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This file is part of FlashBackClient
 *
 * FlashBackClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FlashBackClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FlashBackClient.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <flashbackclient/logging/logger.h>

#include <any>
#include <expected>
#include <filesystem>
#include <string>
#include <type_traits>
#include <unordered_map>

namespace FlashBackClient
{
    struct SettingError
    {
        enum class Type
        {
            notFound,
            noValue,
            wrongType
        };

        Type errorType;
        // cppcheck-suppress unusedStructMember
        std::string message;
        std::string storedType;
        std::string targetType;

        std::string ToString() const
        {
            switch (errorType)
            {
                case Type::notFound: return "Item not found in key";
                case Type::noValue: return "std::any is empty";
                case Type::wrongType:
                    return "Stored type '" + storedType +
                           "' cannot be converted to requested type '" +
                           targetType + '\'';
            }
        }
    };

    class SettingManager
    {
    public:
        explicit SettingManager(const std::filesystem::path& path) :
            _settingFile(path)
        {
        }

        virtual ~SettingManager() = default;

        virtual bool Initialize();
        virtual bool Shutdown();

        const inline std::unordered_map<std::string, std::any>&
        GetSettings() const
        {
            return _settings;
        }

        inline void
        SetSettings(const std::unordered_map<std::string, std::any>& settings)
        {
            _settings = settings;
        }

        template<typename T>
        inline std::expected<T, SettingError>
        GetSettingValue(const std::string& name) const
        {
            if (_settings.find(name) == _settings.end())
            {
                LOG_INFO("not in map");
                return std::unexpected(SettingError {
                    SettingError::Type::notFound,
                    "Value '" + name + "' not found in settings map"});
            }

            const std::any& a = _settings.at(name);

            // Exact type match
            if (const auto* value = std::any_cast<T>(&a)) return *value;

            if (!a.has_value())
            {
                return std::unexpected(
                    SettingError {SettingError::Type::noValue,
                                  "No stored value for '" + name + '\''});
            }

            // TODO: switch to std::variant to use visitation
            // return std::visit(
            //     [name](auto&& arg) -> std::expected<T, SettingError>
            //     {
            //         using storedType = std::decay_t<decltype(arg)>;
            //         if constexpr (std::is_convertible_v<storedType, T>)
            //         {
            //             return static_cast<T>(arg);
            //         }
            //         return std::unexpected(SettingError{
            //             SettingError::Type::wrongType,
            //             "Cannot implicitly convert for '" + name + '\'',
            //             typeid(storedType).name(),
            //             typeid(T).name()
            //         });
            //     }, a
            // );

            return std::unexpected(SettingError {
                SettingError::Type::wrongType,
                "Cannot implicitly convert for '" + name + '\'',
                typeid(std::decay_t<decltype(a)>).name(), typeid(T).name()});
        }

        inline void SetSettingValue(const std::string& name, std::any value)
        {
            _settings[name] = value;
        }

    protected:
        std::unordered_map<std::string, std::any> _settings;

        std::filesystem::path _settingFile;

    private:
        void loadSettings(const std::filesystem::path& path);

        std::string expandHomeDirectory(const std::string& path);
    };
} // namespace FlashBackClient
