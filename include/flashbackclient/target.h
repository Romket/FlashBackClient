#pragma once

#include <flashbackclient/managers/rulemanager.h>
#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/trigger.h>

#include <memory>
#include <vector>

namespace FlashBackClient
{
    class Rule;

    class Target : public RuleManager, public SettingManager
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
        static std::unique_ptr<Target> Create(Args&&... args)
        {
            return std::make_unique<Target>(std::forward<Args>(args)...,
                                            Private());
        }

        bool Initialize() override;
        bool Shutdown() override;

        void CheckRules(const std::vector<Triggers>& givenTriggers = {});

        bool IsIgnored(const std::filesystem::path& path);

        void Checked();

    private:
        void resetRules();
        void checkUploadAndDownload();

        std::vector<std::unique_ptr<Rule>> checkOverrideRules();
        bool checkRule(const std::unique_ptr<Rule>& defaultRule);

        std::string globToRegex(const std::string& glob);

        bool upload();
        bool download();
    };
} // namespace FlashBackClient
