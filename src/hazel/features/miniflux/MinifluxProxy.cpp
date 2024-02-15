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

void hazel::InitMinifluxProxy(HazelCore& server) {
    CROW_ROUTE(server.getApp(), "/api/miniflux/webhook")
        .methods(crow::HTTPMethod::POST)
        (HAZEL_CALLBACK_BINDING(minifluxForwardToProxy));

}

void hazel::minifluxForwardToProxy(HazelCore& server, crow::request& req, crow::response& res) {
    auto auth = req.get_header_value("Authorization"); // Thank you httpbin
    if (auth == "" || !auth.starts_with("Basic ")) {
        res.code = 400;
        HAZEL_JSON(res);
        res.end("{\"message\": \"Malformed or missing authorization header\"}");
        return;
    }

    auto b64 = auth.substr(auth.find(' '));

    const auto pl = (int) (3 * b64.size() / 4);
    auto output = reinterpret_cast<unsigned char *>(calloc(pl + 1, 1));
    const auto ol = EVP_DecodeBlock(output, reinterpret_cast<const unsigned char *>(b64.c_str()), b64.size());

    std::string sOut = reinterpret_cast<char*>(output);

    auto split = stc::string::split(sOut, ":", 1);
    if (pl != ol || !sOut.find(":") || split.size() != 2) {
        res.code = 500;
        spdlog::info("{} from {} is a sus payload", auth, req.remote_ip_address);
        HAZEL_JSON(res);
        res.end(R"({"message": "Malformed authorisation header. Try again"})");
        return;
    }

    auto username = split.at(0);
    auto password = split.at(1);

    auto& conf = server.getConfig();
    auto proxyIt = conf.miniflux_proxies.find(username);
    if (proxyIt == conf.miniflux_proxies.end() || proxyIt->second.passphrase != password) {
        res.code = 404;
        spdlog::info("{} attempted to access undefined webhook", req.remote_ip_address);
        HAZEL_JSON(res);
        res.end(R"({"message": "Webhook not found"})");
        return;
    }

    // TODO error handling
    nlohmann::json data = nlohmann::json::parse(req.body);
    spdlog::info("{}", data.dump());
    auto evType = data.at("event_type").get<std::string>();

    if (proxyIt->second.isEventEnabled(evType)) {
        auto& proxy = proxyIt->second;
        spdlog::info("Forwarding webhook from {} (IP: {})", username, req.remote_ip_address);

        if (data.contains("entry")) {
            auto entry = data.at("entry");
            auto pr = _detail::minifluxPublish(proxy, entry);
            if (!pr.has_value()) {
                spdlog::error("Failed to parse entry");

                HAZEL_JSON(res);
                res.code = 400;
                res.end(R"({"message": "Unknown format"})");
                return;
            } else if (pr->status_code >= 400) {
                spdlog::error("Failed to parse or send entry {}: {}", data.at("feed").at("id").get<int>(), pr->text);

                HAZEL_JSON(res);
                res.code = 400;
                res.end(R"({"message": "Unknown format"})");
                return;
            }
        } else if (data.contains("entries")) {
            for (auto& entry : data.at("entries")) {
                auto pr = _detail::minifluxPublish(proxy, entry);
                // TODO: make sure to return a bad code if there is a sufficient number:tm: of failures
                if (!pr.has_value()) {
                    spdlog::error("Failed to parse entry");

                    HAZEL_JSON(res);
                    res.code = 400;
                    res.end(R"({"message": "Unknown format"})");
                    return;
                } else if (pr->status_code >= 400) {
                    spdlog::error("Failed to parse or send entry {}: {}", data.at("feed").at("id").get<int>(), pr->text);
                }
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

std::optional<cpr::Response> hazel::_detail::minifluxPublish(const MinifluxProxy& proxy, const nlohmann::json& entry) {
    auto data = _detail::minifluxParse(proxy, entry);
    if (!data.has_value()) {
        return std::nullopt;
    }
    return cpr::Post(
        cpr::Url{proxy.receiverUrl},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body{data->dump()}
    );
}

std::optional<nlohmann::json> hazel::_detail::minifluxParse(const MinifluxProxy& proxy, const nlohmann::json& entry) {
    auto url = entry.value("url", "");
    if (url == "") {
        return std::nullopt;
    }
    if (proxy.format == "discord") {
        return nlohmann::json {
            {"username", "Miniflux via Hazel"},
            {"content", url}
        };
    } 
    return std::nullopt;
}
