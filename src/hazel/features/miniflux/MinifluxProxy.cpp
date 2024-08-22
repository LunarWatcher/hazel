#include "MinifluxProxy.hpp"
#include "crow/app.h"
#include "crow/common.h"
#include "hazel/server/Config.hpp"
#include "nlohmann/json_fwd.hpp"

#include <hazel/server/Hazel.hpp>
#include <hazel/meta/Macros.hpp>

#include <spdlog/spdlog.h>
#include <stc/StringUtil.hpp>
#include <cpr/cpr.h>

#include <openssl/evp.h>

void hazel::InitMinifluxProxy(HazelCore& server) {
    CROW_ROUTE(server.getApp(), "/api/miniflux/webhook")
        .methods(crow::HTTPMethod::POST)
        (HAZEL_CALLBACK_BINDING(minifluxForwardToProxy));

}

void hazel::minifluxForwardToProxy(HazelCore& server, crow::request& req, crow::response& res) {
    auto auth = req.get_header_value("Authorization"); // Thank you httpbin
    if (auth.empty() || !auth.starts_with("Basic ")) {
        res.code = 400;
        HAZEL_JSON(res);
        res.end("{\"message\": \"Malformed or missing authorization header\"}");
        return;
    }

    auto b64 = auth.substr(auth.find(' ') + 1);

    if (b64.size() % 4 != 0) {
        res.code = 400;
        HAZEL_JSON(res);
        res.end(R"s({"message": "Malformed or missing authorization header. (Did you correctly base64 encode?)"})s");
        return;
    }

    // "For every 4 input bytes exactly 3 output bytes will be produced"
    // Hence the magic 3 / 4 division.
    // b64 is guaranteed to be divisible by 4 as well
    const auto pl = (int) std::ceil(3 * b64.size() / 4);
    //auto *output = reinterpret_cast<unsigned char *>(calloc(pl + 1, 1));
    std::basic_string<unsigned char> output(pl, '\0');
    const auto ol = EVP_DecodeBlock(
        output.data(),
        reinterpret_cast<const unsigned char *>(b64.c_str()),
        (int) b64.size()
    );
    std::string sOut(
        output.begin(),
        // Cut off excess null byte resulting in bad password matching
        output.end() - 1
    );

    auto split = stc::string::split(sOut, ":", 1);
    if (pl != ol || split.size() != 2) {
        res.code = 500;
        spdlog::info("{} from {} is a sus payload", auth, req.remote_ip_address);
        HAZEL_JSON(res);
        res.end(R"({"message": "Malformed authorisation header. Try again"})");
        return;
    }

    auto username = split.at(0);
    auto password = split.at(1);

    const auto& conf = server.getConfig();
    auto proxyIt = conf.miniflux_proxies.find(username);
    if (proxyIt == conf.miniflux_proxies.end() || proxyIt->second.passphrase != password) {
        res.code = 404;
        if (proxyIt == conf.miniflux_proxies.end()) {
            spdlog::info("{} attempted to access undefined webhook ({})", req.remote_ip_address, username);
        } else {
            spdlog::info("{} failed to log into {}", req.remote_ip_address, username);
        }
        HAZEL_JSON(res);
        res.end(R"({"message": "Webhook not found"})");
        return;
    }

    // TODO error handling
    nlohmann::json data = nlohmann::json::parse(req.body);
    const auto& proxy = proxyIt->second;

    //spdlog::info("{}", data.dump());
    auto evType = data.at("event_type").get<std::string>();
    auto adapter = conf.adapters.at(proxy.adapter);
    auto adapterConfig = proxyIt->second.adapter_config.value_or(nlohmann::json{});

    if (proxyIt->second.isEventEnabled(evType)) {
        spdlog::info("Forwarding webhook from {} (IP: {})", username, req.remote_ip_address);

        if (data.contains("entry")) {
            auto entry = data.at("entry");
            auto url = entry.at("url");
            adapter->execute(url, adapterConfig);

        } else if (data.contains("entries")) {
            for (auto& entry : data.at("entries")) {
                auto url = entry.at("url");
                adapter->execute(url, adapterConfig);
            }

        } else {
            spdlog::info("No fucking clue how to parse {}", data.dump(4));

            HAZEL_JSON(res);
            res.code = 500;
            res.end(R"({"message": "Unknown format"})");
            return;
        }
    }
    // If the event isn't enabled, return 200 anyway. There's no point in sending an error to the server
    // when there isn't an error.

    HAZEL_JSON(res);
    res.end(R"({"message": "ok"})");
}

