#include <flashbackclient/scheduler.h>

#include <flashbackclient/defs.h>

#include <yaml-cpp/yaml.h>

namespace FlashBackClient
{
    void Scheduler::checkStartup()
    {
        for (const auto& target : _targets)
        {
            target.CheckRules(Triggers::on_startup);
        }
    }

    void Scheduler::loadTargets(const std::filesystem::path& path, int depth)
    {
        if (depth > 10)
            throw std::invalid_argument("Too many directories");

        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.path().is_directory())
                loadTargets(entry.path(), depth + 1);
            else if (!entry.is_regular_file() || entry.path().extension() != ".yaml")
                continue;
            else if (entry.path().is_regular_file())
                _targets.push_back(Target(entry.path()));
        }
    }
} //namespace FlashBackClient