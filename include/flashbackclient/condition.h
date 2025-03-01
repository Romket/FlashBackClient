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

        bool Initialize();

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
