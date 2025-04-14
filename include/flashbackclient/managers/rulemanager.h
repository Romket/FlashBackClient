/**
 * @file rulemanager.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines the RuleManager class for loading and handling rules
 * @version 0.1
 * @date 2025-03-25
 *
 * @see rulemanager.cpp
 * @sa scheduler.h
 * @sa scheduler.cpp
 * @sa target.h
 * @sa target.cpp
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

#include <flashbackclient/rule.h>
#include <flashbackclient/trigger.h>

#include <memory>
#include <vector>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <vector>

namespace FlashBackClient
{
    class RuleManager
    {
    public:
        explicit RuleManager(const std::filesystem::path& path) :
            _ruleFile(path)
        {
        }

        virtual ~RuleManager() = default;

        virtual bool Initialize();
        virtual bool Shutdown();

        const inline std::vector<std::unique_ptr<Rule>>& GetRules() const
        {
            return _rules;
        }

    protected:
        std::vector<std::unique_ptr<Rule>> _rules;

        std::filesystem::path _ruleFile;

    private:
        void loadRules(const std::filesystem::path& path);
    };
} // namespace FlashBackClient
