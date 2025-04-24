cmake_minimum_required(VERSION 3.15)
project(osu-tracker LANGUAGES C CXX)

# CPU Threads to use
set(CMAKE_BUILD_PARALLEL_LEVEL 4)

set(CMAKE_CXX_STANDARD 20) # Set the C++ standard to C++20
set(CMAKE_C_STANDARD 23) # Set the C standard to C23

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

# Base directories
set(BASE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/includes")

# STATIC LIB
option(USE_STATIC_LIB "Use static library" ON)

file(COPY ${CMAKE_SOURCE_DIR}/www DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/osu-tracker/")
file(COPY ${CMAKE_SOURCE_DIR}/tracker_txt DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/osu-tracker/")

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

file(GLOB_RECURSE CPR_SOURCES CONFIGURE_DEPENDS lib/cpr/*.cpp)
file(GLOB_RECURSE CURL_SOURCES CONFIGURE_DEPENDS lib/curl/lib/*.c)
file(GLOB_RECURSE ZLIB_SOURCES CONFIGURE_DEPENDS lib/zlib/*.c)

file(GLOB_RECURSE CPR_HEADERS CONFIGURE_DEPENDS lib/cpr/*.h)
file(GLOB_RECURSE CURL_HEADERS CONFIGURE_DEPENDS lib/curl/lib/*.h)
file(GLOB_RECURSE ZLIB_HEADERS CONFIGURE_DEPENDS lib/zlib/*.h)

# OUTPUT
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	add_executable(osu-tracker main.cpp
		${CPR_SOURCES}
		${CPR_HEADERS}
		${CURL_SOURCES}
		${ZLIB_SOURCES}
		${CURL_HEADERS}
		${ZLIB_HEADERS}
	)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_executable(osu-tracker main.cpp
		${CPR_SOURCES}
		${CPR_HEADERS}
	)
endif()


# Find system-installed CURL and ZLIB libraries on Linux
if(UNIX AND NOT APPLE)
    find_package(CURL REQUIRED)
    find_package(ZLIB REQUIRED)
    # Include system headers
    include_directories(${CURL_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(osu-tracker PRIVATE ${CURL_LIBRARIES} ${ZLIB_LIBRARIES})
	include_directories(
        ${CMAKE_SOURCE_DIR}/lib/cpr/cpr
        ${CMAKE_SOURCE_DIR}/lib/cpr/include
    )
else()
    # Include bundled libraries
    include_directories(
        ${CMAKE_SOURCE_DIR}/lib/cpr/cpr
        ${CMAKE_SOURCE_DIR}/lib/cpr/include
        ${CMAKE_SOURCE_DIR}/lib/curl/lib
        ${CMAKE_SOURCE_DIR}/lib/curl/include
        ${CMAKE_SOURCE_DIR}/lib/zlib
    )
endif()

add_definitions(-DCURL_DISABLE_LDAP)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "osu! Tracker" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/osu-tracker")

# WEB SERVER DEFINITIONS 
add_compile_definitions(OSU_TRACKER_ENABLE_WEBSERVER=1) # Legacy flag due to webserver was an optional feature
add_compile_definitions(OSU_TRACKER_WEBSERVER_IP="127.0.0.1")
add_compile_definitions(OSU_TRACKER_WEBSERVER_PORT=10727)

# CMAKE BUILD AND COMPILER DEFINITIONS
add_compile_definitions(OSU_TRACKER_CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RELEASE_BUILD)
endif()

# No MacOS support
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_compile_definitions(_WIN32)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_compile_definitions(__linux__)
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
