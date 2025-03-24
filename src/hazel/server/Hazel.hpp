#pragma once


#include <hazel/meta/Typedefs.hpp>
#include "hazel/config/ConfigState.hpp"
#include "hazel/data/Database.hpp"

#include <chrono>

#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore {
private:

    ConfigState conf;
    Database db;

    Server app;

    std::string assetBaseDir;
    bool sslEnabled;


public:
    HazelCore(const std::string& configRoot);

    const long long startedAt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
        .time_since_epoch()).count();

    void bootstrapDatabase();
    void prepareHandlers();

    /**
     * Takes care of the actual server setup.
     * Does not set up endpoints, only the app metadata.
     */
    void run(bool test);

    static void init();

    const ConfigState& getConfig() { return conf; }
    decltype(app)& getApp() { return app; }

    const std::string& getAssetBaseDir() {
         return assetBaseDir;
    }

    bool isSSLEnabled() { return sslEnabled; }

    static HazelCore& getInstance() {
        static HazelCore server(
#ifdef HAZEL_DEBUG
    "./hazel.conf"
#else
    "/etc/hazel/hazel.conf"
#endif
        );
        return server;
    }

};

}
