#pragma once

#include <flashbackclient/trigger.h>

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <string>
#include <vector>

namespace FlashBackClient
{
    class Helper
    {
    public:
        static std::vector<Rule> LoadRules(const std::filesystem::path& configPath);

        static std::unordered_map<std::string, any> LoadSettings(const std::filesystem::path& configPath);
    
    private:
        static bool loadCases(Rule& rule, const YAML::Node& casesNode);
    };
}