#include <flashbackclient/rule.h>

#include <flashbackclient/condition.h>
#include <flashbackclient/logger.h>
#include <flashbackclient/target.h>
#include <memory>
#include <unordered_map>
#include <vector>

namespace FlashBackClient
{
    Rule::Rule(const Rule* other)
    {
        _id     = other->GetID();
        _name   = other->GetName();
        _action = other->GetAction();
        _isMet  = other->IsMet();

        std::vector<std::unique_ptr<Condition>> conditions;
        for (const auto& condition : other->GetConditions())
        {
            conditions.push_back(std::make_unique<Condition>(condition.get()));
        }

        _conditions = std::move(conditions);
    }

    bool Rule::Initialize(Target* owner)
    {
        LOG_INFO("Initializing rule");

        if (!owner)
        {
            LOG_ERROR("Owner is null");
            return false;
        }

        _owner = owner;

        // cppcheck-suppress useStlAlgorithm
        for (const auto& condition : _conditions)
        {
            if (!condition->Initialize(this)) return false;
        }

        return true;
    }

    bool Rule::LoadCases(const YAML::Node& casesNode)
    {
        if (!casesNode) return false;

        for (const auto& caseNode : casesNode)
        {
            if (!caseNode["id"])
            {
                LOG_WARN("Case has no id");
                continue;
            }

            Triggers trigger = caseNode["id"].as<Triggers>();

            if (trigger == Triggers::none) continue;

            std::unordered_map<std::string, std::any> info;

            if (trigger == Triggers::on_schedule)
            {
                if (!caseNode["times"])
                {
                    LOG_WARN("Case has no times");
                    continue;
                }

                for (const auto& time : caseNode["times"])
                {
                    if (!time["cron_exp"])
                    {
                        LOG_WARN("Time has no cron_exp");
                        continue;
                    }

                    if (info.find("times") == info.end())
                    {
                        info.try_emplace("times", std::vector<std::string>());
                    }

                    auto& times =
                        std::any_cast<std::vector<std::string>&>(info["times"]);
                    times.push_back(time["cron_exp"].as<std::string>());
                }
            }
            else if (trigger == Triggers::after_interval)
            {
                int after_last            = 0;
                int before_next_scheduled = 0;

                if (caseNode["after_last"])
                    after_last = caseNode["after_last"].as<int>();
                if (caseNode["before_next_scheduled"])
                    before_next_scheduled =
                        caseNode["before_next_scheduled"].as<int>();

                info["after_last"]            = after_last;
                info["before_next_scheduled"] = before_next_scheduled;
            }

            _conditions.push_back(
                std::make_unique<Condition>(trigger, info, this));
        }

        if (_conditions.empty())
        {
            LOG_WARN("Rule has no cases");
            return false;
        }

        return true;
    }

    bool Rule::Check(const std::vector<Triggers>& givenTriggers)
    {
        if (_isMet) return true;

        _isMet = true;
        for (const auto& condition : _conditions)
        {
            if (!condition->Check(givenTriggers)) _isMet = false;
        }

        return _isMet;
    }

    void Rule::CheckParent()
    {
        if (!Check()) return;

        _owner->Checked();
    }

    void Rule::Reset()
    {
        _isMet = false;
        for (const auto& condition : _conditions) condition->Reset();
    }
} // namespace FlashBackClient
