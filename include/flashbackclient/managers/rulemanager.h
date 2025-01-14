#pragma once

#include <flashbackclient/trigger.h>

#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <unordered_map>

namespace FlashBackClient
{
    class RuleManager
    {
    public:
        RuleManager(const std::filesystem::path& path) : loadRules(path) {}
        virtual ~RuleManager() = default;

        void CheckRules(const std::vector<Triggers>& givenTriggers = {});

        inline std::unordered_map<Rule, bool> GetRules() { return _rules; }

    protected:
        virtual void afterCheck();

    private:
        void loadRules(const std::filesystem::path& path);

        bool loadCases(Rule& rule, const YAML::Node& casesNode);
        
        bool checkConditions(const Rule& rule, const std::vector<Triggers>& givenTriggers = {});

        bool checkTrigger(Trigger trigger, const std::vector<Trigger> givenTriggers = {});

        std::unordered_map<Rule, bool> _rules;
    };
} //namespace FlashBackClient