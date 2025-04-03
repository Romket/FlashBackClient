/**
 * @file condition.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines a condition, the smallest unit of rule checking
 * @version 0.1
 * @date 2025-03-25
 *
 * @see condition.cpp
 * @see rule.h
 * @sa rule.cpp
 * @sa target.h
 * @sa target.cpp
 * @sa trigger.h
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

#pragma once

#include <any>
#include <filesystem>
#include <flashbackclient/trigger.h>
#include <unordered_map>
#include <vector>

namespace FlashBackClient
{
    class Rule;

    // TODO: something better than std::any for info
    class Condition
    {
    public:
        Condition();
        explicit Condition(
            Triggers                                         name,
            const std::unordered_map<std::string, std::any>& info, Rule* owner,
            bool isMet = false) :
            _name(name), _info(info), _owner(owner), _isMet(isMet)
        {
        }

        explicit Condition(Condition* const other);

        bool Initialize(Rule* owner);

        const Triggers&              GetName() const { return _name; }
        const std::filesystem::path& GetPath() const { return _path; }
        const std::unordered_map<std::string, std::any>& GetInfo() const
        {
            return _info;
        }
        const bool IsMet() const { return _isMet; }
        Rule*&     GetOwner() { return _owner; }

        void SetName(Triggers name) { _name = name; }
        void SetInfo(const std::unordered_map<std::string, std::any>& info)
        {
            _info = info;
        }
        void SetStatus(bool isMet) { _isMet = isMet; }
        void ChangeOwner(Rule* owner) { _owner = owner; }

        void CheckParent();
        bool Check(const std::vector<Triggers>& givenTriggers = {});

        void Reset();

    private:
        bool checkFileChanges();
        bool checkSchedule();

        Triggers _name;

        std::filesystem::path _path;

        std::unordered_map<std::string, std::any> _info;

        Rule* _owner;

        bool _isMet;
    };
} // namespace FlashBackClient
