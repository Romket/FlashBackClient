/**
 * @file rule.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines a rule, a container of conditions
 * @version 0.1
 * @date 2025-03-25
 *
 * @see rule.cpp
 * @see target.h
 * @see condition.h
 *
 * @copyright Copyright (c) 2025 Luke Houston
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <flashbackclient/condition.h>
#include <flashbackclient/trigger.h>
#include <memory>
#include <vector>
#include <yaml-cpp/yaml.h>

namespace FlashBackClient
{
    class Target;

    class Rule
    {
    public:
        Rule();
        explicit Rule(int id, const std::string& name, Actions action,
                      Target* owner = nullptr) :
            _id(id), _name(name), _action(action), _owner(owner), _isMet(false)
        {
        }

        explicit Rule(const Rule* other);

        // TODO: notify owners?
        ~Rule() = default;

        bool Initialize(Target* owner = nullptr);
        bool Shutdown();

        bool Check(const std::vector<Triggers>& givenTriggers = {});
        void CheckParent();

        bool IsEqual(const Rule* other)
        {
            return other->GetID() == _id && other->GetName() == _name;
        }

        const int          GetID() const { return _id; }
        const std::string& GetName() const { return _name; }
        const Actions      GetAction() const { return _action; }
        const std::vector<std::unique_ptr<Condition>>& GetConditions() const
        {
            return _conditions;
        }
        Target*    GetOwner() { return _owner; }
        const bool IsMet() const { return _isMet; }

        bool IsEmpty() { return _conditions.empty(); }

        void SetID(int id) { _id = id; }
        void SetName(const std::string& name) { _name = name; }
        void SetAction(Actions action) { _action = action; }
        void SetStatus(bool isMet) { _isMet = isMet; }

        void AddCondition(std::unique_ptr<Condition> condition)
        {
            _conditions.push_back(std::move(condition));
        }

        void ChangeOwner(Target* owner) { _owner = owner; }

        bool LoadCases(const YAML::Node& casesNode);

        void Reset();

    private:
        int         _id;
        std::string _name;

        // cppcheck-suppress unusedStructMember
        Actions _action;

        // cppcheck-suppress unusedStructMember
        std::vector<std::unique_ptr<Condition>> _conditions;
        Target*                                 _owner;

        bool _isMet;
    };
} // namespace FlashBackClient
