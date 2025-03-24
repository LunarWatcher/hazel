#include "hazel/server/Hazel.hpp"

#include "spdlog/spdlog.h"
#ifndef HAZEL_DEBUG
#include "spdlog/cfg/env.h"
#endif

#include "CLI/CLI.hpp"
#include <filesystem>

int main(int argc, char** argv) {
#ifdef HAZEL_DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::cfg::load_env_levels();
#endif
    CLI::App app {
        "Homelab metaserver"
    };

    app.require_subcommand();

    auto* run = app.add_subcommand("run", "Starts the server");
    auto* configWizard = app.add_subcommand("setup", "Starts an interactive setup system");

    std::filesystem::path configDir = 
#ifdef HAZEL_DEBUG
        "./data/";
#else
        "/opt/hazel/data/";
#endif

    app.add_option("-d,--data-dir", configDir, "Where to source data and config from. This is where the database goes, and where scripts go");

    CLI11_PARSE(app, argc, argv);

    std::filesystem::create_directories(configDir);

    if (configWizard->parsed()) {
        std::cout << "Not currently in use or development" << std::endl;
    } else if (run->parsed()) {
        hazel::HazelCore::init();
    }
}
