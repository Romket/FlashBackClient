/**
 * @file platform_listener.h
 * @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
 * @brief Determines the listener version to use
 * @version 0.1
 * @date 2025-03-25
 *
 * @see inotify_listener.h
 * @see listener.h
 * @sa inotify_listener.cpp
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

#if defined(__linux__)
#include "inotify/inotify_listener.h"
namespace FlashBackClient
{
    using PlatformListener = InotifyListener;
} // namespace FlashBackClient
#else
#error "Unsupported platform"
#endif
