/**
 * @file condition.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Implements the Condition class with trigger creation and detection
 * @version 0.1
 * @date 2025-03-28
 *
 * @see condition.h
 * @sa rule.h
 * @sa target.h
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This file is part of FlashBackClient
 *
 * FlashBackClient is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FlashBackClient is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FlashBackClient.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <flashbackclient/condition.h>

#include <flashbackclient/logging/logger.h>
#include <flashbackclient/rule.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>
#include <listener/platform_listener.h>

#include <algorithm>

namespace FlashBackClient
{
    Condition::Condition(Condition* const other)
    {
        _name  = other->GetName();
        _path  = other->GetPath();
        _info  = other->GetInfo();
        _owner = other->GetOwner();
        _isMet = other->IsMet();
    }

    bool Condition::Initialize(Rule* owner)
    {
        LOG_INFO("Initializing condition");

        if (!owner)
        {
            LOG_ERROR("Owner is null");
            return false;
        }
        _owner = owner;

        Target* target = _owner->GetOwner();
        if (!target)
        {
            LOG_ERROR("Target is null");
            return false;
        }

        if (const auto path =
                _owner->GetOwner()->GetSettingValue<std::string>("path");
            path.has_value())
        {
            _path = *path;
        }
        else
        {
            LOG_ERROR("{}", path.error().ToString());
            LOG_ERROR("  {}", path.error().message);
        }

        LOG_INFO("Got path");

        if (_name == Triggers::on_file_change)
        {
            ListenerCreateInfo info;
            info.owner      = this;
            info.baseTarget = _owner->GetOwner();
            info.path       = _path;

            LOG_INFO("Adding listener for path: {}", info.path.string());

            if (!ServiceLocator::Get<PlatformListener>()->AddListener(info))
                return false;
        }
        else if (_name == Triggers::on_schedule)
        {
            if (_info.find("times") == _info.end())
            {
                LOG_WARN("No times found");
                return true;
            }
            for (const auto& time :
                 std::any_cast<std::vector<std::string>&>(_info["times"]))
            {
                LOG_INFO("Adding time: {}", time);

                ScheduledTime info;
                info.cron  = time;
                info.owner = this;
                info.type  = ScheduledRuleType::atTimePoint;

                ServiceLocator::Get<Scheduler>()->AddTimePoint(info, true);
            }
        }

        return true;
    }

    bool Condition::Check(const std::vector<Triggers>& givenTriggers)
    {
        if (_isMet) return true;

        // cppcheck-suppress useStlAlgorithm
        for (const auto& trigger : givenTriggers)
        {
            if (trigger == _name)
            {
                _isMet = true;
                return true;
            }
            else if (trigger == Triggers::on_file_change)
            {
                if (checkFileChanges()) return true;
            }
            else if (trigger == Triggers::on_schedule)
            {
                if (checkSchedule()) return true;
            }
        }

        return false;
    }

    void Condition::Reset()
    {
        _isMet = false;

        if (_name == Triggers::on_file_change)
        {
            ServiceLocator::Get<PlatformListener>()->SetListenerStatus(
                _path, StatusEnum::active);
        }
        else if (_name == Triggers::on_schedule)
        {
            ServiceLocator::Get<Scheduler>()->SetTimeStatus(this, false);
            ServiceLocator::Get<Scheduler>()->ResetTimePoint(this);
        }
    }

    void Condition::CheckParent()
    {
        if (!_isMet) return;

        _owner->CheckParent();
    }

    bool Condition::checkFileChanges()
    {
        // cppcheck-suppress useStlAlgorithm
        for (const auto& listener :
             ServiceLocator::Get<PlatformListener>()->GetListeners())
        {
            auto it = std::find(listener.conditions.begin(),
                                listener.conditions.end(), this);

            if (it != listener.conditions.end() &&
                listener.status == StatusEnum::modified)
            {
                return true;
            }
        }

        return false;
    }

    bool Condition::checkSchedule()
    {
        // TODO: std::any_of
        // cppcheck-suppress useStlAlgorithm
        for (const auto& time :
             ServiceLocator::Get<Scheduler>()->GetScheduledTimes())
        {
            if (time.owner == this && time.updated) return true;
        }

        return false;
    }
} // namespace FlashBackClient
