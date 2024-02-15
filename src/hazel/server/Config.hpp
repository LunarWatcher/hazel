#pragma once

#include "nlohmann/detail/macro_scope.hpp"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace hazel {

/**
 * Config for the miniflux webhook proxy feature
 */
struct MinifluxProxy {
    std::vector<std::string> events;
    std::string passphrase;
    std::string receiverUrl;
    std::string format;

    bool isEventEnabled(const std::string& ev) const {
        return std::find(events.begin(), events.end(), ev) != events.end();
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(MinifluxProxy,
        events,
        passphrase,
        receiverUrl,
        format
    );
};

/**
 * Config for the server itself.
 */
struct ServerConfig {
    uint16_t port;
    
    ServerConfig() : port(6905) {}

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ServerConfig,
        port
    );
};

/**
 * Combined config struct
 */
struct Config {
    std::map<std::string, MinifluxProxy> miniflux_proxies;
    ServerConfig server;

};

extern void from_json(const nlohmann::json& i, Config& o);

}
