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
        explicit RuleManager(const std::filesystem::path& path) : _ruleFile(path) {}
        virtual ~RuleManager() = default;

        virtual bool Initialize();

        const inline std::unordered_map<Rule, bool>& GetRules() const { return _rules; }

    protected:
        std::unordered_map<Rule, bool> _rules;

        std::filesystem::path _ruleFile;

    private:
        void loadRules(const std::filesystem::path& path);

        bool loadCases(Rule& rule, const YAML::Node& casesNode);
    };
} //namespace FlashBackClient
