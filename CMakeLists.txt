cmake_minimum_required(VERSION 3.28)
project(hazel)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message(FATAL_ERROR "MacOS? Really? No. Get a real OS. https://www.youtube.com/watch?v=_6dbNzFD0zM")
elseif (NOT WIN32)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Wno-c++11-narrowing")
else()
    message(FATAL_ERROR "Windows is not supported, as this is server software. (It might still compile if you remove the FATAL_ERROR message, but you'll be running it without support). On the bright side, you're not using a crapple machine :)")
endif()

find_library(UUID_LIB uuid REQUIRED)
find_package(OpenSSL REQUIRED)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(SPDLOG_FMT_EXTERNAL ON CACHE STRING "" FORCE)
# TODO: Disable this and use system curl. WE're already using system openssl
set(CURL_ZLIB ON CACHE STRING "" FORCE)

set(CROW_FEATURES ssl compression CACHE STRING "" FORCE)
set(CROW_INSTALL OFF CACHE STRING "" FORCE)

include (FetchContent)

# This has to be sourced from github because the apt version is grossly out of date,
# and doesn't support std::optional out of the box :facedesk:
FetchContent_Declare(pqxx
    GIT_REPOSITORY https://github.com/jtv/libpqxx
    GIT_TAG 7.8.1
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(crowcpp
    GIT_REPOSITORY https://github.com/CrowCpp/Crow)
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 9.1.0)
FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.0
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(nlohmann
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2)
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.11.0)
FetchContent_Declare(stc
    GIT_REPOSITORY https://github.com/LunarWatcher/stc)
FetchContent_Declare(cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG main
)

# Must be included before asio [not currently applicable, as asio is an external system dependency now]

# This is overkill, but I'm copying all of this in from a now dead project also based around crowcpp,
# so I'll deal with this when I'm awake
FetchContent_MakeAvailable(crowcpp)
FetchContent_MakeAvailable(pqxx)
FetchContent_MakeAvailable(nlohmann)
FetchContent_MakeAvailable(fmt)
FetchContent_MakeAvailable(spdlog)
FetchContent_MakeAvailable(cpr)
FetchContent_MakeAvailable(stc)
FetchContent_MakeAvailable(cli11)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAZEL_DEBUG")
endif()

# Set up versioning {{{
find_program(GIT git)
if (GIT)
    execute_process(COMMAND ${GIT} describe --tags --always
        OUTPUT_VARIABLE HAZEL_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Detected version: ${HAZEL_VERSION}")
else()
    message(WARNING "Git not found. Defaulting to unknown version. This will not have an effect on the program beyond a missing version")
    set(HAZEL_VERSION "VERSION UNKNOWN. Who doesn't have Git? It's the 2020s. Install it already")
endif()
add_definitions(-DHAZEL_VERSION="${HAZEL_VERSION}")
# }}}
add_definitions(-DCROW_ENFORCE_WS_SPEC=1)

add_subdirectory(src)
add_subdirectory(tests EXCLUDE_FROM_ALL)

add_custom_target(test
    COMMAND tests
    DEPENDS tests
    COMMENT "Test hazel")
# vim:ft=cmake
