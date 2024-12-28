#include <flashbackclient/target.h>
#include <flashbackclient/helper.h>

#include <iostream>

namespace FlashBackClient
{
    Target::Target(std::string& configPath)
    {
        _targetRules = Helper::LoadRules(configPath);
        _targetSettings = Helper::LoadSettings(configPath);

        if (!_targetSettings["name"])
            throw std::invalid_argument("Target has no name");

        _targetName = _targetSettings["name"].as<std::string>();
    }

    void Target::Upload()
    {
        // TODO
    }

    void Target::Download()
    {
        // TODO
    }
} //namespace FlashBackClient