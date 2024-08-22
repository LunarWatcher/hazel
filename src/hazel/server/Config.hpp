#pragma once

#include "hazel/automation/Adapter.hpp"
#include "hazel/data/DashboardStructs.hpp"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include <hazel/json/SerialiserImpl.hpp>

namespace hazel {

/**
 * Config for the miniflux webhook proxy feature
 */
struct MinifluxProxy {
    std::vector<std::string> events;
    std::string passphrase;
    std::string adapter;
    std::string secret;
    std::optional<nlohmann::json> adapter_config;

    bool isEventEnabled(const std::string& ev) const {
        return std::find(events.begin(), events.end(), ev) != events.end();
    }

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(MinifluxProxy,
        events,
        passphrase,
        secret,
        adapter,
        adapter_config
    );
};

/**
 * Config for the server itself.
 */
struct ServerConfig {
    uint16_t port;
    
    ServerConfig() : port(6906) {}

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ServerConfig,
        port
    );
};

struct DashboardLink {
    LinkDynamicApp type;
    std::string name,
        url;
    std::optional<nlohmann::json> config;
};

struct DashboardWeather {
    std::string location;
};

struct DashboardStatModule {
    StatModuleTypes type;

};

struct DashboardConfig {
    std::vector<DashboardLink> links;

    DashboardWeather weather;
};

/**
 * Combined config struct
 */
struct Config {
    ServerConfig server;
    DashboardConfig dashboard;

    std::map<std::string, MinifluxProxy> miniflux_proxies;
    std::map<std::string, std::shared_ptr<Adapter>> adapters;

};

extern void from_json(const nlohmann::json& i, Config& o);
extern void from_json(const nlohmann::json& i, DashboardConfig& o);
extern void from_json(const nlohmann::json& i, DashboardLink& o);

}
