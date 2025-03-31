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
    message(FATAL_ERROR "FlashBack Client currently only supports unix systems. Future Windows support is planned.")
endif()

if (UNIX AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(SPDLOG_CLOCK_COARSE)
endif()

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
