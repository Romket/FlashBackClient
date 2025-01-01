#pragma once

#include <flashbackclient/trigger.h>

#include <filesystem>
#include <unordered_map>
#include <vector>

namespace FlashBackClient
{
    class Target
    {
    public:
        Target(const std::filesystem::path& path);

        ~Target() = default;

        template <typename... Args>
        void CheckRules(Args... args)

    private:
        bool checkConditions(const Rule& rule, Args... args);

        bool upload();
        bool download();

        std::string _targetName;

        std::unordered_map<std::string, any> _targetSettings;
        std::unordered_map<std::string, Rule> _targetRules;
    };
} //namespace FlashBackClient
