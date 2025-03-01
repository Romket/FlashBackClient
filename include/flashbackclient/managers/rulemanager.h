#pragma once

#include <flashbackclient/rule.h>
#include <flashbackclient/trigger.h>

#include <memory>
#include <vector>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <vector>

namespace FlashBackClient
{
    class RuleManager
    {
    public:
        explicit RuleManager(const std::filesystem::path& path) :
            _ruleFile(path)
        {
        }

        virtual ~RuleManager() = default;

        virtual bool Initialize();
        virtual bool Shutdown();

        const inline std::vector<std::unique_ptr<Rule>>& GetRules() const
        {
            return _rules;
        }

    protected:
        std::vector<std::unique_ptr<Rule>> _rules;

        std::filesystem::path _ruleFile;

    private:
        void loadRules(const std::filesystem::path& path);
    };
} // namespace FlashBackClient
