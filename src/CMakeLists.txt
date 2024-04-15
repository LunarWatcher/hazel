set (SOURCE_FILES
    # Middlwares
    hazel/middlewares/BaseMiddlewares.cpp

    # Endpoints
    hazel/features/miniflux/MinifluxProxy.cpp
    hazel/features/WebCore.cpp
    hazel/features/webcore/Dashboard.cpp

    # Adapters
    hazel/automation/adapters/DiscordAdapter.cpp
    hazel/automation/adapters/NtfyAdapter.cpp

    # Utils
    hazel/json/JsonImpl.cpp

    # Dashboard stuff
    hazel/data/DashboardDataProvider.cpp
    hazel/data/dashboard/DashboardUpdaters.cpp

    # Server (just the core shit)
    hazel/server/Hazel.cpp
)

list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN/../lib")

add_library(hazelsrc STATIC ${SOURCE_FILES})
add_executable(hazel hazel/Main.cpp)

target_include_directories(hazelsrc PUBLIC ./)

target_link_libraries(hazelsrc
    PUBLIC
    stc
    fmt::fmt
    zlib
    cpr::cpr
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    Crow::Crow
    ${UUID_LIB}
    OpenSSL::Crypto
    pqxx
)
target_link_libraries(hazel hazelsrc)

# Copying web content
add_custom_target(webcontent
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/www
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/www ${CMAKE_BINARY_DIR}/www
    COMMENT "Copying web to output dir"
)
add_custom_target(webcontentdebug
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/www ${CMAKE_BINARY_DIR}/www-debug
    COMMENT "Symlink debug symlink repo"
)
add_dependencies(hazelsrc webcontent webcontentdebug)

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

# vim:ft=cmake
