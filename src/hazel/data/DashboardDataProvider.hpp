#pragma once

#include "hazel/data/DashboardStructs.hpp"
#include "hazel/server/Config.hpp"
#include "hazel/sync/RWContainer.hpp"
#include <optional>
#include <thread>
#include <shared_mutex>
#include <nlohmann/json.hpp>

namespace hazel {

class DashboardDataProvider {
private:
    /**
     * Underlying struct for the data.
     * This is used to update and manage the data 
     * in a non-JSON format.
     */
    DashboardData dataArchive;

    RWContainer<std::string> jsonCache;

    /**
     * The thread in charge of running the update() function
     */
    std::thread updateProcessor;

    void update();
public:
    DashboardDataProvider(DashboardConfig& conf);

    std::string getJsonData();
};

}
