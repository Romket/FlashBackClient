#include "scheduler.h"

#include <flashbackclient/defs.h>

#include <yaml-cpp/yaml.h>

namespace FlashBackClient
{
    bool Scheduler::Initialize()
    {
        loadTargets();
        for (const auto& target : _targets)
        {
            target->Initialize();
        }

        CheckRules({Triggers::on_startup});

        return true;
    }

    void Scheduler::afterCheck(const std::vector<Triggers>& givenTriggers)
    {
        for (const auto& target : _targets)
        {
            target->CheckRules(givenTriggers);
        }
    }

    void Scheduler::loadTargets(const std::filesystem::path& path, int depth)
    {
        if (depth > RECURSION_LIMIT)
            throw std::invalid_argument("Too many directories");

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (std::filesystem::is_directory(entry.path()))
                loadTargets(entry.path(), depth + 1);
            else if (!std::filesystem::is_regular_file(entry.path()) || entry.path().extension() != ".yaml")
                continue;
            else
                _targets.push_back(std::make_unique<Target>(Target(entry.path())));
        }
    }
} //namespace FlashBackClient
