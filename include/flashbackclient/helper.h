#pragma once

#include <flashbackclient/trigger.h>

#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>

namespace FlashBackClient
{
    class Helper
    {
    public:
        static std::vector<Rule> LoadRules(std::string& configPath);

        static std::unordered_map<std::string, any> LoadSettings(std::string& configPath);
    
    private:
        static bool loadCases(Rule& rule, const YAML::Node& casesNode);
    };
}