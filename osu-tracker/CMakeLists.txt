cmake_minimum_required(VERSION 3.15)
project(osu-tracker LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Base directories
set(BASE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/includes")

# STATIC LIB
option(USE_STATIC_LIB "Use static library" ON)

# zlib
file(GLOB ZLIB_SRC CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/lib/zlib/*.c
)

add_library(zlib STATIC ${ZLIB_SRC})
target_include_directories(zlib PUBLIC ${CMAKE_SOURCE_DIR}/lib/zlib)

# curl
file(GLOB CURL_SRC CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/lib/curl/lib/*.c
)

add_library(curl STATIC ${CURL_SRC})
target_include_directories(curl PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/curl/include
    ${CMAKE_SOURCE_DIR}/lib/curl/lib
)

target_compile_definitions(curl PUBLIC CURL_STATICLIB)
target_link_libraries(curl PUBLIC zlib)

# cpr
file(GLOB CPR_SRC CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/lib/cpr/cpr/*.cpp
)

add_library(cpr STATIC ${CPR_SRC})
target_include_directories(cpr PUBLIC
    ${CMAKE_SOURCE_DIR}/lib/cpr/include
    ${CMAKE_SOURCE_DIR}/lib/cpr
)

target_link_libraries(cpr PUBLIC curl)

# Debug/Release-specific paths
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    #set(BUILD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/osu-tracker/includes/Debug/")
    #set(BUILD_LIB_PATH "${CMAKE_SOURCE_DIR}/osu-tracker/libs/LIB_Debug/")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    #set(BUILD_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/osu-tracker/includes/Release/")
    #set(BUILD_LIB_PATH "${CMAKE_SOURCE_DIR}/osu-tracker/libs/LIB_Release/")
endif()

file(COPY ${CMAKE_SOURCE_DIR}/www DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/osu-tracker/")

# Add the include directories for the project
include_directories(${BASE_INCLUDE_PATH} ${BUILD_INCLUDE_PATH})

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


# OUTPUT
add_executable(osu-tracker main.cpp "header/webserver.h" "includes/crow/middleware.h")
target_link_libraries(osu-tracker PRIVATE cpr zlib curl pthread dl)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "osu! Tracker")

# WEB SERVER DEFINITIONS 
add_compile_definitions(OSU_TRACKER_ENABLE_WEBSERVER=1) # Legacy flag due to webserver was an optional feature
add_compile_definitions(OSU_TRACKER_WEBSERVER_IP="127.0.0.1")
add_compile_definitions(OSU_TRACKER_WEBSERVER_PORT=727)

# CMAKE BUILD AND COMPILER DEFINITIONS
add_compile_definitions(OSU_TRACKER_CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	add_compile_definitions(DEBUG_BUILD)
endif()

# No MacOS support
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	add_compile_definitions(WINDOWS)
	add_compile_definitions(_WIN32)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_compile_definitions(LINUX)
endif()

add_compile_definitions(OSU_TRACKER_CMAKE_GENERATOR="${CMAKE_GENERATOR}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_STANDARD="C++ ${CMAKE_CXX_STANDARD}")
add_compile_definitions(OSU_TRACKER_CMAKE_VERSION="${CMAKE_VERSION}")
add_compile_definitions(OSU_TRACKER_CMAKE_MINIMUM_REQUIRED_VERSION="${CMAKE_MINIMUM_REQUIRED_VERSION}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER_ARCHITECTURE_ID="${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}")
add_compile_definitions(OSU_TRACKER_CMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}")

# OSU_TRACKER PROJECT STATIC STRINGS
add_compile_definitions(OSU_TRACKER_NAME="osu! Tracker")
add_compile_definitions(OSU_TRACKER_VERSION="v0.1")
add_compile_definitions(OSU_TRACKER_RELEASE_TYPE="Alpha")
add_compile_definitions(OSU_TRACKER_CREATOR="Railgun")
add_compile_definitions(OSU_TRACKER_PROFILE="https://osu.ppy.sh/users/13817114")
add_compile_definitions(OSU_TRACKER_GITHUB="https://github.com/nyaruku/osu-tracker")

# LIBRARY INFO
add_compile_definitions(CROW_REPOSITORY_URL="https://github.com/CrowCpp/Crow/tree/e8b125159ec5e61384187e36bbf6d14f1fad504f")
add_compile_definitions(LIBCURL_REPOSITORY_URL="https://github.com/curl/curl")
add_compile_definitions(ASIO_REPOSITORY_URL="https://github.com/chriskohlhoff/asio")
add_compile_definitions(CPR_REPOSITORY_URL="https://github.com/libcpr/cpr")
add_compile_definitions(ZLIB_REPOSITORY_URL="https://github.com/madler/zlib")
add_compile_definitions(NLOHMANNJSON_REPOSITORY_URL="https://github.com/nlohmann/json")