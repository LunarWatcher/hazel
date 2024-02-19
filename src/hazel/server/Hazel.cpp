#include "Hazel.hpp"
#include "hazel/features/WebCore.hpp"
#include "hazel/features/miniflux/MinifluxProxy.hpp"

#include <fstream>
#include <spdlog/spdlog.h>

namespace hazel {

HazelCore::HazelCore() {
#ifdef HAZEL_DEBUG
    std::string path = "./hazel.conf";
#else
    std::string path = "/etc/hazel/hazel.conf";
#endif
    // TODO: search path for multiple files, though this is primarily useful in debug mode,
    // so not sure if it really adds any value.
    std::ifstream ifs(path);
    if (!ifs) {
        throw std::runtime_error("You need to create /etc/hazel/hazel.conf before running the program.");
    }

    conf = nlohmann::json::parse(ifs);
}

void HazelCore::bootstrapDatabase() {
    spdlog::info("This is where database stuff will go");
}


void HazelCore::prepareHandlers() {
#ifdef HAZEL_DEBUG
    if (std::filesystem::exists("www-debug")) {
        // Symlinked repo; only respected for fast iterative updates when dealing with
        // webdev
        crow::mustache::set_global_base("www-debug");
        crow::mustache::set_base("www-debug");
        assetBaseDir = "www-debug";
        spdlog::info("Using www-debug");
    } else {
        crow::mustache::set_global_base("www");
        crow::mustache::set_base("www");
        assetBaseDir = "www";
        spdlog::info("Using www");
    }
    app.loglevel(crow::LogLevel::DEBUG);
#else
    crow::mustache::set_global_base("www");
    crow::mustache::set_base("www");
    assetBaseDir = "www";
#endif
    spdlog::info("Loading endpoints...");

    InitMinifluxProxy(*this);
    InitWebCore(*this);

}

void HazelCore::run(bool test) {
    bootstrapDatabase();
    prepareHandlers();

    app
        .server_name("LunarWatcher/Hazel")
        //.loglevel(crow::LogLevel::Debug)
        .multithreaded()
        .use_compression(crow::compression::algorithm::GZIP)
        .port(
            //conf.data.contains("unittestport") ? conf.data.at("unittestport").get<int>() :
            //(
//#ifdef HAZEL_DEBUG
                //conf.data.value("devport", 1337)
//#else
                //conf.data.contains("ssl") && conf.data.at("ssl").value("enabled", false)
                    //? conf.data.value("sslport", 443)
                    //: conf.data.value("port", 22)
//#endif
            //)
            // TODO: make dynamic
#ifdef HAZEL_DEBUG
            1337
#else
            conf.server.port
#endif
        );

    if (!test) {
        app.run();
    }
}

void HazelCore::init() {
    HazelCore& p = HazelCore::getInstance();

    p.run(false);
}

std::shared_ptr<pqxx::connection> HazelCore::createDBConnection() {
    return staticCreateDBConnection({});
}

// TODO: make this function use the raw Config struct
std::shared_ptr<pqxx::connection> HazelCore::staticCreateDBConnection(const nlohmann::json& /* conf */) {
    throw std::runtime_error("To be implemented when needed at a later, undefined time");
}

}
