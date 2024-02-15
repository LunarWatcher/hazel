#include <hazel/server/Config.hpp>

void hazel::from_json(const nlohmann::json& i, Config& o) {
    if (i.contains("server")) {
        i.at("server").get_to(o.server);
    } 
    if (i.contains("miniflux_proxies")) {
        i.at("miniflux_proxies").get_to(o.miniflux_proxies);
    }
}
