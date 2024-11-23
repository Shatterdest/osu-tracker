cmake_minimum_required(VERSION 3.15)
project(cli-osu-tracker LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Debugging output for architecture
message(STATUS "CMAKE_GENERATOR_PLATFORM: ${CMAKE_GENERATOR_PLATFORM}")
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

SET(GCC_COVERAGE_COMPILE_FLAGS "-std=c++2b")

# Base directories
set(BASE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/includes")
set(BASE_LIB_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/libs")
include_directories(“C:/winlibs-i686/include”)

# Debug/Release-specific paths
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/includes/Debug/")
    set(BUILD_LIB_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/libs/Debug/")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(BUILD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/includes/Release/")
    set(BUILD_LIB_PATH "${CMAKE_SOURCE_DIR}/cli-osu-tracker/libs/Release/")
endif()

# Add the include directories for the project
include_directories(${BASE_INCLUDE_PATH} ${ARCH_INCLUDE_PATH} ${BUILD_INCLUDE_PATH})

# Add the library directories for linking
link_directories(${BASE_LIB_PATH} ${ARCH_LIB_PATH} ${BUILD_LIB_PATH})

# Add subdirectories for project(s)
add_subdirectory(cli-osu-tracker)
