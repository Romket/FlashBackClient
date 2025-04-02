/**
 * @file dualsink.h
 * @author Chase Attebury (Appleberry) (chaseappleberryboi@gmail.com)
 * @brief Defines a sink with different formatting based on log levels
 * @version 0.1
 * @date 2025-03-31
 *
 * @see dualsink.cpp
 * @sa logger.h
 * @sa logger.cpp
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

#include <memory>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace FlashBackClient
{
    class DualLevelSink : public spdlog::sinks::stdout_color_sink_mt
    {
    private:
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _traceSink;
        std::shared_ptr<spdlog::sinks::stdout_color_sink_mt> _warnSink;

    public:
        virtual ~DualLevelSink() = default;
        DualLevelSink();

        void Initialize();

        void log(const spdlog::details::log_msg& msg) override;
        void flush() override;
    };
} // namespace FlashBackClient
