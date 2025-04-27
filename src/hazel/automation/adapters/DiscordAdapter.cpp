#include "DiscordAdapter.hpp"
#include "hazel/automation/Adapter.hpp"
#include "spdlog/spdlog.h"

#include <cpr/cpr.h>

namespace hazel {

DiscordAdapter::DiscordAdapter(const nlohmann::json& config) {
    this->destUrl = config.at("dest_url");
    this->username = config.value("username", "Hazel");
}

void DiscordAdapter::execute(const std::string& content, const Adapter::Extras&, const nlohmann::json& adapterConfig) {
    auto body = nlohmann::json {
        {"username", adapterConfig.value("username", username)},
        {"content", content}
    };

    auto res = cpr::Post(
        cpr::Url{this->destUrl},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{body.dump()}
    );

    if (res.status_code >= 400 || res.status_code < 100) {
        spdlog::error("Failed to push to Discord webhook: {}", res.text);
    }
}

}
