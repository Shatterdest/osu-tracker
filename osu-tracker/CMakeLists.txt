# ----------------------------------- #
# | Project-specific CMakeLists.txt | #
# ----------------------------------- #

# OUTPUT
add_executable(osu-tracker main.cpp "header/webserver.h"  "includes/crow/middleware.h" )
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