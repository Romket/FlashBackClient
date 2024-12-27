#pragma once

#include <flashbackclient/target.h>
#include <flashbackclient/trigger.h>

#include <vector>

namespace FlashBackClient
{
    enum Actions
    {
        no_action,
        download_changed,
        upload_changed,
        sync_files
    };

    enum Triggers
    {
        none,
        on_startup,
        on_shutdown,
        on_file_change,
        on_schedule,
        after_interval
    };

    class Scheduler
    {
    private:
        void checkTargets();
        void loadDefaultRules();

        std::vector<Target> _targets;
        std::vector<Rule> _globalRules;
    };
} //namespace FlashBackClient