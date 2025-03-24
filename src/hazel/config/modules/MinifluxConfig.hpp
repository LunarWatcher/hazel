#include <vector>
#include <string>
#include <optional>
#include <nlohmann/json.hpp>
#include <algorithm>

namespace hazel {

struct MinifluxProxy {
    std::vector<std::string> events;
    std::string passphrase;
    std::string adapter;
    std::string secret;
    std::optional<nlohmann::json> adapter_config;

    bool isEventEnabled(const std::string& ev) const {
        return std::find(events.begin(), events.end(), ev) != events.end();
    }

};

inline void from_json(const nlohmann::json& j, MinifluxProxy& o) {
    j.at("events").get_to(o.events);
    j.at("passphrase").get_to(o.passphrase);
    j.at("adapter").get_to(o.adapter);
    o.secret = j.value("secret", "");
    if (j.contains("adapter_config")) {
        o.adapter_config = j.at("adapter_config");
    }
}

}
