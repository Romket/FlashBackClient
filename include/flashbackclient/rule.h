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

        bool Initialize();
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
        Target*&   GetOwner() { return _owner; }
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
