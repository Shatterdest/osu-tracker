# Project-specific CMakeLists.txt
add_executable(cli-osu-tracker main.cpp "header/webserver.h"  "includes/crow/middleware.h" )
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "osu! Tracker")
add_compile_definitions(OSU_TRACKER_ENABLE_WEBSERVER=1) # Legacy flag due to webserver was an optional feature
add_compile_definitions(OSU_TRACKER_WEBSERVER_IP="127.0.0.1")
add_compile_definitions(OSU_TRACKER_WEBSERVER_PORT=10727)
add_compile_definitions(OSU_TRACKER_DEBUG_BUILD=true)
add_compile_definitions(OSU_TRACKER_NAME="osu! Tracker")
add_compile_definitions(OSU_TRACKER_VERSION="v1.0")
add_compile_definitions(OSU_TRACKER_CREATOR="Railgun")
add_compile_definitions(OSU_TRACKER_PROFILE="https://osu.ppy.sh/users/13817114")