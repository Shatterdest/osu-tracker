cmake_minimum_required(VERSION 3.15)
project(osu-tracker LANGUAGES C CXX)

# Threads to use
set(CMAKE_BUILD_PARALLEL_LEVEL 4)

set(CMAKE_CXX_STANDARD 20) # Set the C++ standard to C++20
set(CMAKE_C_STANDARD 23) # Set the C standard to C23

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# Base directories
set(BASE_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/includes")
# Add the include directories for the project
include_directories(${BASE_INCLUDE_PATH})

# CMAKE
option(USE_STATIC_LIB "Use static library" ON)
option(BUILD_TEST "Build Test" ON) # You can disable compilation of test executable

file(COPY ${CMAKE_SOURCE_DIR}/www DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/osu-tracker/")
file(COPY ${CMAKE_SOURCE_DIR}/tracker_txt DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/osu-tracker/")

# Sources
file(GLOB_RECURSE CPR_SOURCES CONFIGURE_DEPENDS lib/cpr/*.cpp)
file(GLOB_RECURSE ZLIB_SOURCES CONFIGURE_DEPENDS lib/zlib/*.c)

# Headers
file(GLOB_RECURSE CPR_HEADERS CONFIGURE_DEPENDS lib/cpr/*.h)
file(GLOB_RECURSE CURL_HEADERS CONFIGURE_DEPENDS lib/curl/lib/*.h)
file(GLOB_RECURSE ZLIB_HEADERS CONFIGURE_DEPENDS lib/zlib/*.h)

# No MacOS support
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
	add_compile_definitions(_WIN32)
	add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER_ARCHITECTURE_ID="${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}")

	# Static libcurl, libcrypto and libssl (windows only)
	# Using system curl and zlib on linux
	set(LIBCURL_LIBRARY "${CMAKE_SOURCE_DIR}/lib/static/libcurl.lib")
	set(LIBCRYPTO_LIBRARY "${CMAKE_SOURCE_DIR}/lib/static/libcrypto_static.lib")
	set(LIBSSL_LIBRARY "${CMAKE_SOURCE_DIR}/lib/static/libssl_static.lib")
	add_executable(osu-tracker main.cpp
		${CPR_SOURCES}
		${CPR_HEADERS}
		${CURL_HEADERS}
		${ZLIB_SOURCES}
		${ZLIB_HEADERS}
	)
	target_sources(osu-tracker PRIVATE "${CMAKE_SOURCE_DIR}/res/resources.rc")
	if(BUILD_TEST)
		add_executable(test-tracker test.cpp
			${CPR_SOURCES}
			${CPR_HEADERS}
			${CURL_HEADERS}
			${ZLIB_SOURCES}
			${ZLIB_HEADERS}
			includes/catch_amalgamated.cpp
		)
		# static libcurl
		add_definitions(-DCURL_STATICLIB)
		# static link for tracker
		target_link_libraries(osu-tracker PRIVATE crypt32 advapi32 ${LIBCRYPTO_LIBRARY} ${LIBSSL_LIBRARY} ${LIBCURL_LIBRARY})
		# static link for test, ws2_32 is required for catch2
		target_link_libraries(test-tracker PRIVATE ws2_32 crypt32 advapi32 ${LIBCRYPTO_LIBRARY} ${LIBSSL_LIBRARY} ${LIBCURL_LIBRARY} )
	endif()
	# Include bundled libraries
	include_directories(
		${CMAKE_SOURCE_DIR}/lib/cpr/cpr
		${CMAKE_SOURCE_DIR}/lib/cpr/include
		${CMAKE_SOURCE_DIR}/lib/curl/include
		${CMAKE_SOURCE_DIR}/lib/zlib
	)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
	add_compile_definitions(__linux__)
	add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER_ARCHITECTURE_ID="${CMAKE_SYSTEM_PROCESSOR}")
	find_package(CURL REQUIRED)
	find_package(ZLIB REQUIRED)

	add_executable(osu-tracker main.cpp
		${CPR_SOURCES}
		${CPR_HEADERS}
	)
	if(BUILD_TEST)
		add_executable(test-tracker test.cpp
			${CPR_SOURCES}
			${CPR_HEADERS}
			includes/catch_amalgamated.cpp
		)
		target_link_libraries(test-tracker PRIVATE ${CURL_LIBRARIES} ${ZLIB_LIBRARIES})
	endif()

	# Include system headers
	include_directories(${CURL_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS})
	target_link_libraries(osu-tracker PRIVATE ${CURL_LIBRARIES} ${ZLIB_LIBRARIES})
	include_directories(
		${CMAKE_SOURCE_DIR}/lib/cpr/cpr
		${CMAKE_SOURCE_DIR}/lib/cpr/include
	)
endif()

set_target_properties(osu-tracker PROPERTIES OUTPUT_NAME "osu-tracker" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/osu-tracker")
if(BUILD_TEST)
	set_target_properties(test-tracker PROPERTIES OUTPUT_NAME "osu-tracker-test" RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/osu-tracker")
endif()

# CMAKE BUILD AND COMPILER DEFINITIONS
add_compile_definitions(OSU_TRACKER_CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_BUILD)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RELEASE_BUILD)

	if(MSVC)
        # Windows (MSVC) specific optimizations
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /GL")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /GL")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        # Linux (GCC/Clang) specific optimizations
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto")
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -march=native -flto")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -flto")
    endif()
endif()

# WEB SERVER DEFINITIONS 
add_compile_definitions(OSU_TRACKER_ENABLE_WEBSERVER=1) # Legacy flag due to webserver was an optional feature
add_compile_definitions(OSU_TRACKER_WEBSERVER_IP="127.0.0.1")
add_compile_definitions(OSU_TRACKER_WEBSERVER_PORT=10727)

# CMAKE Info
add_compile_definitions(OSU_TRACKER_CMAKE_GENERATOR="${CMAKE_GENERATOR}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_STANDARD="C++ ${CMAKE_CXX_STANDARD}")
add_compile_definitions(OSU_TRACKER_CMAKE_VERSION="${CMAKE_VERSION}")
add_compile_definitions(OSU_TRACKER_CMAKE_MINIMUM_REQUIRED_VERSION="${CMAKE_MINIMUM_REQUIRED_VERSION}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}")
add_compile_definitions(OSU_TRACKER_CMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}")
add_compile_definitions(OSU_TRACKER_CMAKE_SYSTEM_NAME="${CMAKE_SYSTEM_NAME}")

# OSU_TRACKER PROJECT STATIC STRINGS
add_compile_definitions(OSU_TRACKER_NAME="osu! Tracker")
add_compile_definitions(OSU_TRACKER_VERSION="v0.2.1")
add_compile_definitions(OSU_TRACKER_RELEASE_TYPE="Beta")
add_compile_definitions(OSU_TRACKER_CREATOR="Railgun")
add_compile_definitions(OSU_TRACKER_PROFILE="https://osu.ppy.sh/users/13817114")
add_compile_definitions(OSU_TRACKER_GITHUB="https://github.com/nyaruku/osu-tracker")
add_compile_definitions(OSU_TRACKER_DISCORD="https://discord.gg/FN6vauFTGA")

# LIBRARY INFO
add_compile_definitions(CROW_REPOSITORY_URL="https://github.com/CrowCpp/Crow/tree/e8b125159ec5e61384187e36bbf6d14f1fad504f")
add_compile_definitions(LIBCURL_REPOSITORY_URL="https://github.com/curl/curl")
add_compile_definitions(ASIO_REPOSITORY_URL="https://github.com/chriskohlhoff/asio")
add_compile_definitions(CPR_REPOSITORY_URL="https://github.com/libcpr/cpr")
add_compile_definitions(ZLIB_REPOSITORY_URL="https://github.com/madler/zlib")
add_compile_definitions(NLOHMANNJSON_REPOSITORY_URL="https://github.com/nlohmann/json")