cmake_minimum_required(VERSION 3.28)
project(hazel)

set (CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option (LINT "Whether or not to enable clang-tidy checks" OFF)
option (LINT_WARNINGS_ARE_ERRORS "Whether or not to set -warnings-as-errors" OFF)
option (SANITISE "Whether or not to enable -fsanitize=undefined" OFF)

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
set(SQLITECPP_RUN_CPPLINT OFF CACHE STRING "" FORCE)

set(CROW_FEATURES ssl compression CACHE STRING "" FORCE)
set(CROW_INSTALL OFF CACHE STRING "" FORCE)

include (FetchContent)
FetchContent_Declare(crowcpp
    GIT_REPOSITORY https://github.com/CrowCpp/Crow
    GIT_TAG v1.2.0
)
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG 11.0.2
)
FetchContent_Declare(cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
    EXCLUDE_FROM_ALL
)
FetchContent_Declare(nlohmann
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.2
)
FetchContent_Declare(spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.14.1
)
FetchContent_Declare(stc
    GIT_REPOSITORY https://github.com/LunarWatcher/stc
)
FetchContent_Declare(cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11
    GIT_TAG v2.4.2
)
FetchContent_Declare(sqlitecpp
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp
    GIT_TAG 3.3.2
)

FetchContent_MakeAvailable(
    crowcpp
    sqlitecpp
    nlohmann
    fmt
    spdlog
    cpr
    stc
    cli11
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAZEL_DEBUG")
endif()

if (SANITISE)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
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
