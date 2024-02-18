set (SOURCE_FILES
    # Middlwares
    hazel/middlewares/BaseMiddlewares.cpp

    # Endpoints
    hazel/features/miniflux/MinifluxProxy.cpp
    hazel/features/WebCore.cpp

    # Adapters
    hazel/automation/adapters/DiscordAdapter.cpp
    hazel/automation/adapters/NtfyAdapter.cpp

    # Utils
    hazel/json/JsonImpl.cpp

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
# For when there's actual web content
#install(
    #DIRECTORY www
    #DESTINATION .
#)

# vim:ft=cmake
