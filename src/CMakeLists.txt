set (SOURCE_FILES
    # Middlwares
    hazel/middlewares/BaseMiddlewares.cpp

    # Endpoints
    hazel/features/miniflux/MinifluxProxy.cpp

    # Adapters
    hazel/automation/adapters/DiscordAdapter.cpp
    hazel/automation/adapters/NtfyAdapter.cpp

    # Database
    hazel/data/Database.cpp
    hazel/data/db/Migration.cpp

    # Server (just the core shit)
    hazel/server/Hazel.cpp

    # Config
    hazel/config/ConfigState.cpp
)

list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

add_library(hazelsrc STATIC ${SOURCE_FILES})
add_executable(hazel hazel/Main.cpp)

#message(FATAL_ERROR ${CMAKE_CXX_FLAGS})
target_include_directories(hazelsrc PUBLIC ./)

target_link_libraries(hazelsrc
    PUBLIC
    stc
    cpr::cpr
    nlohmann_json::nlohmann_json
    ${UUID_LIB}
    OpenSSL::Crypto
    SQLiteCpp
    CLI11::CLI11
    magpie
)
target_link_libraries(hazel hazelsrc)

# Copying web content
# add_custom_target(webcontent
#     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/www
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/www ${CMAKE_BINARY_DIR}/www
#     COMMENT "Copying web to output dir"
# )
# add_custom_target(webcontentdebug
#     COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/www ${CMAKE_BINARY_DIR}/www-debug
#     COMMENT "Symlink debug symlink repo"
# )
# add_dependencies(hazelsrc webcontent webcontentdebug)

# Doesn't work because no sudo
# Maybe it doesn't make sense to use /etc then? maybe use /opt/hazel/conf/?
# Won't help with the systemd service, but there's an install script for that
#install(DIRECTORY ../etc/hazel ../etc/systemd DESTINATION /etc)
install(TARGETS hazel
    DESTINATION bin)
install(
    DIRECTORY ${CMAKE_BINARY_DIR}/lib
    DESTINATION .
    FILES_MATCHING
        PATTERN *.so*
        PATTERN *.a EXCLUDE
)
install(
    DIRECTORY ${CMAKE_BINARY_DIR}/www
    DESTINATION .
)

if (LINT)
    find_program(
        CLANGTIDY
        NAMES "clang-tidy" "clang-tidy.exe"
        REQUIRED
    )

    set (CLANG_TIDY_COMMAND "${CLANGTIDY}" "--config-file=${CMAKE_CURRENT_SOURCE_DIR}/../.clang-tidy")
    if (LINT_WARNINGS_ARE_ERRORS)
        list (APPEND CLANG_TIDY_COMMAND  -warnings-as-errors=*)
    endif()

    if (WIN32)
        list (APPEND CLANG_TIDY_COMMAND --extra-arg=/EHsc)
    endif()

    message(STATUS "Using CLANG_TIDY_COMMAND=${CLANG_TIDY_COMMAND}")
    set_target_properties(hazelsrc PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()


# vim:ft=cmake
