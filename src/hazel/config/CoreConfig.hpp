#pragma once

#include "nlohmann/json.hpp"
#include <cstdint>

namespace hazel {

struct CoreConfig {
    uint16_t port = 6996;
};

inline void from_json(const nlohmann::json& j, CoreConfig& o) {
    if (j.contains("port")) {
        o.port = j.at("port");
    }
}

}
