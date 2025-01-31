#include <flashbackclient/scheduler.h>

#include <flashbackclient/defs.h>

#include <yaml-cpp/yaml.h>

namespace FlashBackClient
{
    bool Scheduler::Initialize()
    {
        if (!RuleManager::Initialize())
            return false;

        loadTargets();

        for (const auto& target : _targets)
        {
            target->CheckRules({Triggers::on_startup});
        }

        return true;
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
            {
                std::unique_ptr<Target> target = std::make_unique<Target>(Target(entry.path()));
                if(!target->Initialize())
                    continue;
                _targets.push_back(std::move(target));
            }
        }
    }
} //namespace FlashBackClient
