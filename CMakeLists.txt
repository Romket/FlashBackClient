cmake_minimum_required(VERSION 3.12)

project(FlashBackClient
        VERSION 0.0.0.1
        LANGUAGES C CXX
        DESCRIPTION "Fast, cross-device, cloud backup and sync program")

set(CMAKE_CXX_STANDARD 17)

add_executable(FlashBackClient
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

if (NOT UNIX)
    message(FATAL_ERROR "FlashBack Client currently only supports unix systems. Future Windows support is planned.")
