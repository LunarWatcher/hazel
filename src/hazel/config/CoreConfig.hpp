#pragma once

#include "nlohmann/json.hpp"
#include <cstdint>

namespace hazel {

struct CoreConfig {
    /**
     * The port to run Hazel at. This should be a non-80/443 port if using nginx or similar.
     */
    uint16_t port = 6996;
    /**
     * The default path to store the database and other data at
     */
    std::string dataRoot = 
#ifdef HAZEL_DEBUG
        "./";
#else
        "/opt/hazel/dist/data";
#endif
};

inline void from_json(const nlohmann::json& j, CoreConfig& o) {
    if (j.contains("port")) {
        o.port = j.at("port");
    }

    if (j.contains("dataRoot")) {
        o.dataRoot = j.at("dataRoot");
    }
}

}
