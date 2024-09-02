#include "hazel/server/Hazel.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

#include "CLI/CLI.hpp"
#include <filesystem>

int main(int argc, char** argv) {
#ifdef HAZEL_DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::cfg::load_env_levels();
#endif
    CLI::App app{
        "Homelab metaserver"
    };

    std::filesystem::path configDir = 
#ifdef HAZEL_DEBUG
        "./data/";
#else
        "/opt/hazel/data/";
#endif

    app.add_option("-d,--data-dir", configDir, "Where to source data and config from. This is where the database goes, and where scripts go");

    CLI11_PARSE(app, argc, argv);

    std::filesystem::create_directories(configDir);

    hazel::HazelCore::getInstance().init();
}
