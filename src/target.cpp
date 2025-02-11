#include <flashbackclient/target.h>

#include <flashbackclient/logger.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>
#include <listener/platform_listener.h>

#include <algorithm>
#include <regex>
#include <string>

namespace FlashBackClient
{
    bool Target::Initialize()
    {
        if (!RuleManager::Initialize() || !SettingManager::Initialize())
            return false;

        auto activeDefaults = checkOverrideRules();
        _rules.insert(activeDefaults.begin(), activeDefaults.end());

        if (!ServiceLocator::IsProvided<PlatformListener>())
        {
            ServiceLocator::Provide<PlatformListener>(new PlatformListener());
        }

        for (const auto& rule : _rules)
        {
            std::vector<Condition> ruleConditions = rule.first.Conditions;

            for (const auto& condition : ruleConditions)
            {
                if (condition.TriggerName == Triggers::on_file_change)
                {
                    ListenerInfo info;
                    info.Owner = shared_from_this();
                    info.Path  = GetSettingValue<std::string>("path");

                    if (!ServiceLocator::Get<PlatformListener>()->AddListener(
                            info))
                        return false;
                    return true;
                }
            }
        }

        return true;
    }

    bool Target::Shutdown() { return true; }

    void Target::CheckRules(const std::vector<Triggers>& givenTriggers)
    {
        for (const auto& rule : _rules)
        {
            if (rule.first.Conditions.empty()) continue;

            _rules.find(rule.first)->second =
                checkConditions(rule.first, givenTriggers);
        }

        afterCheck();
    }

    bool Target::IsIgnored(const std::filesystem::path& path)
    {
        if (_settings.find("path_ignores") == _settings.end()) return false;

        std::filesystem::path targetPath = GetSettingValue<std::string>("path");
        std::filesystem::path relativePath =
            path.lexically_relative(targetPath).generic_string();

        LOG_INFO("Relative path: {}", relativePath.string());

        auto& ignores =
            std::any_cast<std::vector<std::string>&>(_settings["path_ignores"]);

        for (const auto& ignore : ignores)
        {
            std::string regex = globToRegex(ignore);

            bool notRecursive = regex.rfind("\\./", 0) == 0;
            if (notRecursive) regex.erase(0, 3);
            LOG_INFO("Regex after not recursive check: {}", regex);

            if (regex.back() == '/' || regex.back() == '\\') regex.pop_back();

            int patternDepth = std::ranges::count(regex, '/') + 1;
            LOG_INFO("Pattern depth: {}", patternDepth);

            std::regex re(regex);
            if (std::regex_match(relativePath.generic_string(), re) &&
                !notRecursive)
                return true;

            int depth = std::ranges::count(relativePath.generic_string(), '/');
            for (auto parent = relativePath.parent_path(); !parent.empty();
                 parent      = parent.parent_path(), depth--)
            {
                LOG_INFO("Parent: {}", parent.string());
                LOG_INFO("Depth: {}", depth);
                if (std::regex_match(parent.string(), re) &&
                    (!notRecursive || depth <= patternDepth))
                    return true;
            }
        }

        return false;
    }

    bool Target::checkConditions(const Rule&                  rule,
                                 const std::vector<Triggers>& givenTriggers)
    {
        for (const auto& condition : rule.Conditions)
        {
            if (checkTrigger(condition.TriggerName, givenTriggers)) continue;

            // TODO: nontrivial conditions
            if (condition.TriggerName == Triggers::on_file_change)
            {
                if (ServiceLocator::Get<PlatformListener>()->ListenerExists(
                        GetSettingValue<std::string>("path")) &&
                    checkFileChanges(condition))
                {
                    continue;
                }
            }

            return false;
        }

        for (const auto& condition : rule.Conditions)
        {
            if (condition.TriggerName == Triggers::on_file_change)
            {
                const std::filesystem::path& path =
                    GetSettingValue<std::string>("path");
                ServiceLocator::Get<PlatformListener>()->SetListenerStatus(
                    path, StatusEnum::active);
            }
        }

        LOG_INFO("Condition met");
        return true;
    }

    bool Target::checkFileChanges(const Condition& condition)
    {
        const std::filesystem::path& path =
            GetSettingValue<std::string>("path");

        // cppcheck-suppress useStlAlgorithm
        for (const auto& listener :
             ServiceLocator::Get<PlatformListener>()->GetListeners())
        {
            if (listener.Path == path &&
                listener.Status == StatusEnum::modified)
            {
                return true;
            }
        }
        return false;
    }

    bool Target::checkTrigger(Triggers                     trigger,
                              const std::vector<Triggers>& givenTriggers)
    {
        // TODO: use std::any_of
        // cppcheck-suppress useStlAlgorithm
        for (const auto& givenTrigger : givenTriggers)
        {
            if (givenTrigger == trigger) return true;
        }

        return false;
    }

    void Target::afterCheck(const std::vector<Triggers>& givenTriggers)
    {
        for (const auto& rule : _rules)
        {
            if (!rule.second) continue;

            if (rule.first.Action == Actions::upload_changed ||
                rule.first.Action == Actions::sync_files)
                upload();

            if (rule.first.Action == Actions::download_changed ||
                rule.first.Action == Actions::sync_files)
                download();
        }
    }

    std::unordered_map<Rule, bool> Target::checkOverrideRules()
    {
        auto schedulerRules = ServiceLocator::Get<Scheduler>()->GetRules();

        if (_settings.find("override_rules") == _settings.end())
            return schedulerRules;

        auto overrideRules =
            std::any_cast<std::vector<int>&>(_settings["override_rules"]);

        std::unordered_map<Rule, bool> metDefaults;

        for (const auto& schedulerRule : schedulerRules)
        {
            if (checkRule(schedulerRule.first, overrideRules))
            {
                metDefaults.insert(
                    std::pair<Rule, bool>(schedulerRule.first, true));
            }
        }

        return metDefaults;
    }

    bool Target::checkRule(Rule                    defaultRule,
                           const std::vector<int>& overrideRules)
    {
        // TODO: use std::none_of
        // cppcheck-suppress useStlAlgorithm
        for (int overrideRule : overrideRules)
        {
            if (overrideRule == defaultRule.id) return false;
        }

        return true;
    }

    std::string Target::globToRegex(const std::string& glob)
    {
        std::string regex;
        bool        recursive        = false;
        bool        inCharacterClass = false;
        bool        firstOfClass     = false;

        for (size_t i = 0; i < glob.size(); i++)
        {
            char c = glob[i];
            switch (c)
            {
                case '*':
                    if (i + 1 < glob.size() && glob[i + 1] == '*')
                    {
                        regex += ".*";
                        i++;
                        recursive = true;
                    }
                    else { regex += "[^/]*"; }

                    break;
                case '[':
                    // Check if this is a character class
                    for (size_t j = i + 1; j < glob.size(); j++)
                    {
                        if (glob[j] == ']' && j > i + 1)
                        {
                            inCharacterClass = true;
                            break;
                        }
                        else if (glob[j] == '[' || glob[j] == '\\' ||
                                 glob[j] == '/')
                            break;
                    }

                    if (!inCharacterClass)
                    {
                        regex += "\\[";
                        break;
                    }

                    regex += '[';
                    i++;
                    firstOfClass = true;

                    if (glob[i] == ']')
                    {
                        regex += "\\]";
                        i++;
                        firstOfClass = false;
                    }

                    while (i < glob.size() && glob[i] != ']')
                    {
                        if (firstOfClass && (glob[i] == '!' || glob[i] == '^'))
                        {
                            regex += '^';
                        }
                        else if (!firstOfClass && glob[i] == '-' &&
                                 i + 1 < glob.size() && glob[i + 1] != ']')
                        {
                            regex += '-';
                        }
                        else { regex += glob[i]; }

                        i++;
                        firstOfClass = false;
                    }
                    regex += ']';
                    inCharacterClass = false;
                    break;
                case '?': regex += "[^/]"; break;
                case '\\': regex += '/'; break;
                case '.':
                case ']':
                case '(':
                case ')':
                case '{':
                case '}': regex += '\\';
                default: regex += c; break;
            }
        }

        LOG_INFO("Regex: {}", recursive ? "^" + regex + "$" : regex);

        return recursive ? "^" + regex + "$" : regex;
    }

    bool Target::upload()
    {
        // TODO
        return true;
    }

    bool Target::download()
    {
        // TODO
        return true;
    }
} // namespace FlashBackClient
