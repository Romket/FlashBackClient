### @file CMakeLists.txt
### @author Luke Houston (Romket or RomketBoi) (lukehouston08@gmail.com)
### @brief Main CMakeLists.txt file for the FlashBackClient project
### @version 0.1
### @date 2025-03-28
###
### @copyright Copyright (c) 2025 Luke Houston
###
### This file is part of FlashBackClient
###
### FlashBackClient is free software: you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation, either version 3 of the License, or
### (at your option) any later version.
###
### FlashBackClient is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with FlashBackClient.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.12)

project(FlashBackClient
        VERSION 0.0.0.1
        LANGUAGES C CXX
        DESCRIPTION "Fast cross-device cloud backup and sync program")

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_INSTALL_PREFIX "/usr/local/bin/")

add_subdirectory(lib/croncpp)
add_subdirectory(lib/yaml-cpp)
add_subdirectory(lib/spdlog)

add_definitions(-DSPDLOG_ACTIVE_LEVEL=0)

add_executable(flashbackclient
    ${CMAKE_CURRENT_SOURCE_DIR}/src/condition.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/configs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/helper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/listener/inotify/inotify_listener.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logging/dualsink.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logging/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/rulemanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/settingmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/rule.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scheduler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/signal_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(flashbackclient yaml-cpp spdlog croncpp)

if (NOT UNIX)
    message(FATAL_ERROR "FlashBack Client currently only supports unix systems.
            Future Windows support is planned.")
endif()

# spdlog definition to use a less accurate clock on linux release versions in
# exchange for performance
if (UNIX AND CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(SPDLOG_CLOCK_COARSE)
endif()

# spdlog definitions to remove unused things to theoretically improve
# performance
add_compile_definitions(
    SPDLOG_NO_SOURCE_LOC
    SPDLOG_NO_THREAD_ID
    SPDLOG_NO_ATOMIC_LEVELS
    SPDLOG_DISABLE_DEFAULT_LOGGER
)

target_compile_options(flashbackclient PRIVATE -g)

install(TARGETS flashbackclient
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    RENAME flashbackclient
)
