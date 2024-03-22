#pragma once

#include "hazel/data/DashboardStructs.hpp"
#include <optional>
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

    std::shared_mutex lock;

    std::string cachedJson;

    void update();
public:
    std::string getJsonData();
};

}
