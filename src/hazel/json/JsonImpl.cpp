#include "hazel/automation/adapters/DiscordAdapter.hpp"
#include "hazel/automation/adapters/NtfyAdapter.hpp"
#include <hazel/server/Config.hpp>

void hazel::from_json(const nlohmann::json& i, Config& o) {
    if (i.contains("server")) {
        i.at("server").get_to(o.server);
    } 
    if (i.contains("miniflux_proxies")) {
        i.at("miniflux_proxies").get_to(o.miniflux_proxies);
    }

    if (i.contains("adapters")) {
        for (auto& [adapterId, conf] : i.at("adapters").items()) {
            auto& type = conf.at("type");
            if (type == "discord") {
                o.adapters.insert({
                    adapterId,
                    std::make_shared<DiscordAdapter>(
                        conf
                    )
                });
            } else if (type == "ntfy") {
                o.adapters.insert({
                    adapterId,
                    std::make_shared<NtfyAdapter>(
                        conf
                    )
                });
            }
        }
    }

    if (i.contains("dashboard")) {
        i.at("dashboard").get_to(o.dashboard);
    }
}

void hazel::from_json(const nlohmann::json& i, DashboardConfig& o) {
    if (i.contains("links")) {
        i.at("links").get_to(o.links);
    }
}

