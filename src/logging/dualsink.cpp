/**
 * @file logger.cpp
 * @author Chase Attebury (Appleberry) (chaseappleberryboi@gmail.com)
 * @brief Defines a custom sink that has different coloring based on log level
 * @version 0.1
 * @date 2025-03-31
 *
 * @see dualsink.h
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

#include <flashbackclient/logging/dualsink.h>

namespace FlashBackClient
{
    DualLevelSink::DualLevelSink()
    {
        _traceSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        _traceSink->set_level(spdlog::level::trace);
        _traceSink->set_pattern("%Y-%m-%d %H:%M:%S.%e [%^%l%$] %v");

        _warnSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        _warnSink->set_level(spdlog::level::warn);
        _warnSink->set_pattern("%^%Y-%m-%d %H:%M:%S.%e [%l] %v%$");
    }

    void DualLevelSink::log(const spdlog::details::log_msg& msg)
    {
        if(msg.level >= spdlog::level::warn)
        {
            _warnSink->log(msg);
        }
        else 
        {
            _traceSink->log(msg);
        }
    }

    void DualLevelSink::flush()
    {
        _traceSink->flush();
        _warnSink->flush();
    }
} // namespace FlashBackClient
