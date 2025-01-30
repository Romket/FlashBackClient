#pragma once

#include <flashbackclient/managers/rulemanager.h>

#include <flashbackclient/defs.h>
#include <flashbackclient/target.h>
#include <flashbackclient/trigger.h>

#include <filesystem>
#include <memory>
#include <vector>


namespace FlashBackClient
{
    class Scheduler : public RuleManager
    {
    public:
        Scheduler() : RuleManager(SCHEDULER_CONFIG_FILE_PATH) {}
        virtual ~Scheduler() = default;

        bool Initialize() override;

    private:
        void loadTargets(const std::filesystem::path& path = TARGET_DEF_DIR, int depth = 0);

        // cppcheck-suppress unusedStructMember
        std::vector<std::unique_ptr<Target>> _targets;
    };
} //namespace FlashBackClient
