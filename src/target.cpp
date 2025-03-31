/**
 * @file target.cpp
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Implements the target class for managing rules and uploading or
 * downloading files
 *
 * @version 0.1
 * @date 2025-03-28
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

#include <flashbackclient/target.h>

#include <flashbackclient/logger.h>
#include <flashbackclient/rule.h>
#include <flashbackclient/scheduler.h>
#include <flashbackclient/service_locator.h>
#include <iterator>
#include <listener/platform_listener.h>

#include <algorithm>
#include <memory>
#include <regex>
#include <string>
#include <vector>

namespace FlashBackClient
{
    bool Target::Initialize()
    {
        LOG_INFO("Initializing new target");

        if (!RuleManager::Initialize() || !SettingManager::Initialize())
            return false;
        LOG_INFO("Initialized parents");

        auto activeDefaults = checkOverrideRules();
        _rules.insert(_rules.end(),
                      std::make_move_iterator(activeDefaults.begin()),
                      std::make_move_iterator(activeDefaults.end()));

        activeDefaults.clear();

        LOG_INFO("Initializing listener");
        if (!ServiceLocator::IsProvided<PlatformListener>())
        {
            ServiceLocator::Provide<PlatformListener>(new PlatformListener());
        }

        LOG_INFO("Initializing rules");
        // cppcheck-suppress useStlAlgorithm
        for (const auto& rule : _rules)
        {
            if (!rule->Initialize(this)) return false;
        }

        return true;
    }

    bool Target::Shutdown() { return true; }

    void Target::CheckRules(const std::vector<Triggers>& givenTriggers)
    {
        if (!givenTriggers.empty())
        {
            for (const auto& rule : _rules)
            {
                if (rule->IsEmpty()) continue;

                rule->Check(givenTriggers);
            }
        }

        checkUploadAndDownload();
        resetRules();
    }

    bool Target::IsIgnored(const std::filesystem::path& path)
    {
        if (_settings.find("path_ignores") == _settings.end()) return false;

        std::filesystem::path targetPath;
        if (const auto path = GetSettingValue<std::string>("path");
            path.has_value())
        {
            targetPath = *path;
        }
        else
        {
            LOG_ERROR("{}", path.error().ToString());
            LOG_ERROR("  {}", path.error().message);
        }

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

    void Target::Checked()
    {
        checkUploadAndDownload();
        resetRules();
    }

    void Target::resetRules()
    {
        for (const auto& rule : _rules)
        {
            if (rule->IsMet()) rule->Reset();
        }
    }

    void Target::checkUploadAndDownload()
    {
        bool uploaded   = false;
        bool downloaded = false;

        for (const auto& rule : _rules)
        {
            if (!rule->IsMet()) continue;

            LOG_INFO("Condition met");

            if (rule->GetAction() == Actions::upload_changed ||
                rule->GetAction() == Actions::sync_files && !uploaded)
            {
                uploaded = upload();
            }

            if (rule->GetAction() == Actions::download_changed ||
                rule->GetAction() == Actions::sync_files && !downloaded)
            {
                downloaded = download();
            }
        }
    }

    std::vector<std::unique_ptr<Rule>> Target::checkOverrideRules()
    {
        auto& schedulerRules = ServiceLocator::Get<Scheduler>()->GetRules();

        bool override = _settings.find("override_rules") != _settings.end();

        std::vector<std::unique_ptr<Rule>> metDefaults;
        for (const auto& schedulerRule : schedulerRules)
        {
            if (!override || checkRule(schedulerRule))
            {
                std::unique_ptr<Rule> metRule =
                    std::make_unique<Rule>(schedulerRule.get());

                metDefaults.push_back(std::move(metRule));
            }
        }

        return metDefaults;
    }

    bool Target::checkRule(const std::unique_ptr<Rule>& defaultRule)
    {
        const auto& overrideRules =
            std::any_cast<std::vector<int>&>(_settings["override_rules"]);
        // TODO: use std::none_of
        // cppcheck-suppress useStlAlgorithm
        for (int overrideRule : overrideRules)
        {
            if (overrideRule == defaultRule->GetID()) return false;
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
                    if (i > 1 && glob[i - 1] == '\\') { regex += '*'; }
                    else if (i + 1 < glob.size() && glob[i + 1] == '*')
                    {
                        regex += ".*";
                        i++;
                        recursive = true;
                    }
                    else { regex += "[^/]*"; }

                    break;
                case '[':
                    if (i > 1 && glob[i - 1] == '\\')
                    {
                        regex += '[';
                        break;
                    }

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
                case '?':
                    if (i > 1 && glob[i - 1] == '\\') { regex += '?'; }
                    else { regex += "[^/]"; }
                    break;
                case '\\':
                    if (i == 1 || glob[i - 1] != '\\') { regex += '\\'; }
                    break;
                case '.':
                case ']':
                case '(':
                case ')':
                case '{':
                case '}':
                    if (i == 1 || glob[i - 1] != '\\') { regex += '\\'; }
                default: regex += c; break;
            }
        }

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
