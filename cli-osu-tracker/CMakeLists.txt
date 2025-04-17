# Project-specific CMakeLists.txt
add_executable(cli-osu-tracker main.cpp "header/webserver.h" "header/logger.h" "includes/crow/middleware.h" "header/global.h")
add_compile_definitions(ENABLE_WEBSERVER=1) # Not ready yet