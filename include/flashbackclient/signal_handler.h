/**
 * @file signal_handler.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Defines a class for handling system signals
 * @version 0.1
 * @date 2025-03-25
 *
 * @see signal_handler.cpp
 * @sa main.cpp
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

namespace FlashBackClient
{
    class SignalHandler
    {
    public:
        static void Register();

    private:
        static void handle(int signum);

        static const char* getSignalString(int signum);
        static bool        isError(int signum);
        static void        logStackTrace();
    };
} // namespace FlashBackClient
