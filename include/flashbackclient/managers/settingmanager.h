#pragma once

#include <flashbackclient/logger.h>

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
            LOG_INFO("Getting setting value {}", name);
            if (_settings.find(name) == _settings.end())
            {
                LOG_INFO("not in map");
                return std::unexpected(SettingError {
                    SettingError::Type::notFound,
                    "Value '" + name + "' not found in settings map"});
            }

            LOG_INFO("Segfault test 1");

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
