/**
 * @file target.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines a class for managing a single target
 * @version 0.1
 * @date 2025-03-25
 *
 * @see target.cpp
 * @see condition.h
 * @see rulemanager.h
 * @see rule.h
 * @see settingmanager.h
 * @sa condition.cpp
 * @sa rulemanager.cpp
 * @sa rule.cpp
 * @sa settingmanager.cpp
 * @sa scheduler.h
 * @sa scheduler.cpp
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

#include <flashbackclient/managers/rulemanager.h>
#include <flashbackclient/managers/settingmanager.h>

#include <flashbackclient/trigger.h>

#include <memory>
#include <vector>

namespace FlashBackClient
{
    class Rule;

    class Target : public RuleManager, public SettingManager
    {
    private:
        struct Private
        {
            explicit Private() = default;
        };

    public:
        explicit Target(const std::filesystem::path& path, Private) :
            RuleManager(path), SettingManager(path)
        {
        }

        virtual ~Target() = default;

        template<typename... Args>
        static std::unique_ptr<Target> Create(Args&&... args)
        {
            return std::make_unique<Target>(std::forward<Args>(args)...,
                                            Private());
        }

        bool Initialize() override;
        bool Shutdown() override;

        void CheckRules(const std::vector<Triggers>& givenTriggers = {});

        bool IsIgnored(const std::filesystem::path& path);

        void Checked();

    private:
        void resetRules();
        void checkUploadAndDownload();

        std::vector<std::unique_ptr<Rule>> checkOverrideRules();
        bool checkRule(const std::unique_ptr<Rule>& defaultRule);

        std::string globToRegex(const std::string& glob);

        bool upload();
        bool download();
    };
} // namespace FlashBackClient
