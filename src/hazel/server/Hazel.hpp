#pragma once


#include <hazel/meta/Typedefs.hpp>
#include "Config.hpp"
#include "hazel/data/DashboardDataProvider.hpp"
#include "hazel/data/Database.hpp"

#include <chrono>
#include <memory>
#include <system_error>

#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore {
private:

    Config conf;
    Database db;

    Server app;

    std::string assetBaseDir;
    bool sslEnabled;
    std::shared_ptr<DashboardDataProvider> dashData;

    HazelCore();

public:

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

    const Config& getConfig() { return conf; }
    decltype(app)& getApp() { return app; }

    const std::string& getAssetBaseDir() {
         return assetBaseDir;
    }

    bool isSSLEnabled() { return sslEnabled; }

    DashboardDataProvider& getDashData() { 
        return *dashData;
    }

    static HazelCore& getInstance() {
        static HazelCore server;
        return server;
    }

};

}
