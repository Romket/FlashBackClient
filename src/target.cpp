#include "target.h"

#include <flashbackclient/service_locator.h>

#include <iostream>

namespace FlashBackClient
{
    void Target::afterCheck()
    {
        auto schedulerRules = ServiceLocator::Get<RuleManager>()->GetRules();
        for (const auto& rule : schedulerRules)
        {
            //for (const auto& rule : )
        }
        // TODO: check rules are met and use upload, download, and sync
    }

    bool Target::upload()
    {
        // TODO
        return true;
    }

    bool Target::download()
    {
        // TODO
        return true;
    }
} //namespace FlashBackClient