/**
 * @file helper.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Miscellaneous helper functions
 * @version 0.2
 * @date 2025-04-01
 *
 * @see helper.cpp
 * @sa main.cpp
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

namespace FlashBackClient
{
    enum class ArgsResult
    {
        cont,
        err,
        exit
    };

    class Helper
    {
    public:
        static void       DisplayNotice();
        static ArgsResult ProcessCommandLineArgs(int argc, char** argv);
    };
} // namespace FlashBackClient
