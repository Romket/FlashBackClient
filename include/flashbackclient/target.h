#pragma once

#include <flashbackclient/managers/rulemanager.h>
#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/trigger.h>

#include <memory>
#include <vector>

namespace FlashBackClient
{
    class Target :
        public RuleManager,
        public SettingManager,
        public std::enable_shared_from_this<Target>
    {
    private:
        struct Private
        {
            explicit Private() = default;
        };

    public:
        explicit Target(const std::filesystem::path& path, Private) :
            RuleManager(path), SettingManager(path)
        {
        }

        virtual ~Target() = default;

        template<typename... Args>
        static std::shared_ptr<Target> Create(Args&&... args)
        {
            return std::make_shared<Target>(std::forward<Args>(args)...,
                                            Private());
        }

        bool Initialize() override;
        bool Shutdown() override;

        void CheckRules(const std::vector<Triggers>& givenTriggers = {});

    private:
        void afterCheck(const std::vector<Triggers>& givenTriggers = {});

        std::unordered_map<Rule, bool> checkOverrideRules();
        bool checkRule(Rule defaultRule, const std::vector<int>& overrideRules);

        bool checkConditions(const Rule&                  rule,
                             const std::vector<Triggers>& givenTriggers = {});

        bool checkTrigger(Triggers                     trigger,
                          const std::vector<Triggers>& givenTriggers = {});

        bool checkFileChanges(const Condition& condition);

        bool upload();
        bool download();
    };
} // namespace FlashBackClient
