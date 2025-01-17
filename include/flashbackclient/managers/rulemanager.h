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
        RuleManager(const std::filesystem::path& path);
        virtual ~RuleManager() = default;

        virtual bool Initialize() { return true; }

        void CheckRules(const std::vector<Triggers>& givenTriggers = {});

        inline std::unordered_map<Rule, bool> GetRules() { return _rules; }

    protected:
        virtual void afterCheck() {}

        std::unordered_map<Rule, bool> _rules;

    private:
        void loadRules(const std::filesystem::path& path);

        bool loadCases(Rule& rule, const YAML::Node& casesNode);
        
        bool checkConditions(const Rule& rule, const std::vector<Triggers>& givenTriggers = {});

        bool checkTrigger(Triggers trigger, const std::vector<Triggers>& givenTriggers = {});
    };
} //namespace FlashBackClient
