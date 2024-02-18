#include "NtfyAdapter.hpp"
#include "cpr/cpr.h"
#include "cpr/ssl_options.h"
#include "cpr/verbose.h"
#include "spdlog/spdlog.h"

namespace hazel {

NtfyAdapter::NtfyAdapter(const nlohmann::json& config) {
    this->destUrl = config.at("dest_url");
    this->defaultTopic = config.at("default_topic");
    this->authHeader = config.at("api_token");
}

void NtfyAdapter::execute(const std::string& content, const nlohmann::json& adapterConfig) {
    auto topic = adapterConfig.value("topic", defaultTopic);
    auto priority = adapterConfig.value("priority", 3);
    auto title = adapterConfig.value("title", "Hazel update");

    cpr::Header headers;

    if (adapterConfig.contains("tags")) {
        headers["Tags"] = adapterConfig.at("tags");
    }
    headers["Priority"] = std::to_string(priority);
    headers["Title"] = title;

    headers["Content-Type"] = "application/x-www-form-urlencoded";
    cpr::Body body{content};
    auto url = cpr::Url {
        this->destUrl + "/" + topic
    };
    spdlog::info("URL: {}", url.str());
    headers["Authorization"] = "Bearer " + this->authHeader;

    auto res = cpr::Post(
        url,
        body,
        headers,
        cpr::VerifySsl(0)
    );
    spdlog::info("{}", res.text);
    if (res.status_code >= 400 && res.status_code >= 100) {
        spdlog::error("Failed to push to Ntfy ({}): {}", res.url.str(), res.text);
    }
}
}
