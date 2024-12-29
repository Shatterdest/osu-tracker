cmake_minimum_required(VERSION 3.15)
project(cli-osu-tracker LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Base directories
set(BASE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/includes")
set(BASE_LIB_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/libs")

# STATIC LIB
option(USE_STATIC_LIB "Use static library" ON)

# Debug/Release-specific paths
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #set(BUILD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/includes/Debug/")
    set(BUILD_LIB_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/libs/LIB_Debug/")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #set(BUILD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/includes/Release/")
    set(BUILD_LIB_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/libs/LIB_Release/")
endif()

# Add the include directories for the project
include_directories(${BASE_INCLUDE_PATH} ${BUILD_INCLUDE_PATH})

# Add the library directories for linking
link_directories(${BASE_LIB_PATH} ${BUILD_LIB_PATH})

# Add subdirectories for project
add_subdirectory(cli-osu-tracker)

# Debug
message(STATUS "#################################################")
message(STATUS "#################################################")
message(STATUS "CMAKE_SYSROOT: ${CMAKE_SYSROOT}")
message(STATUS "BASE_INCLUDE_PATH: ${BASE_INCLUDE_PATH}")
message(STATUS "BASE_LIB_PATH: ${BASE_LIB_PATH}")
message(STATUS "BUILD_INCLUDE_PATH: ${BUILD_INCLUDE_PATH}")
message(STATUS "BUILD_LIB_PATH: ${BUILD_LIB_PATH}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "#################################################")
message(STATUS "#################################################")