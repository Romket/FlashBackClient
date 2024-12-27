#pragma once

namespace FlashBackClient
{
    struct Trigger
    {
        std::string TriggerName;
        std::unordered_map<std::string, any> TriggerInfo = nullptr;
    };

    struct Rule
    {
        Actions Action;
        std::vector<Triggers> Conditions;
    };
} //namespace FlashBackClient