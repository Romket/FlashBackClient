cmake_minimum_required(VERSION 3.12)

project(FlashBackClient
        VERSION 0.0.0.1
        LANGUAGES C CXX
        DESCRIPTION "Fast cross-device cloud backup and sync program")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_PREFIX "/usr/local/bin/")

add_subdirectory(lib/yaml-cpp)
add_subdirectory(lib/spdlog)

add_executable(flashbackclient
    ${CMAKE_CURRENT_SOURCE_DIR}/src/configs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scheduler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/target.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/rulemanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/managers/settingmanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/listener/inotify/inotify_listener.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(flashbackclient yaml-cpp spdlog)

if (NOT UNIX)
    message(FATAL_ERROR "FlashBack Client currently only supports unix systems. Future Windows support is planned.")
endif()

install(TARGETS flashbackclient
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    RENAME flashbackclient
)
