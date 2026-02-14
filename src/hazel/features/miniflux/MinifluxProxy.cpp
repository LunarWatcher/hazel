#include "MinifluxProxy.hpp"
#include "magpie/transfer/StatusCode.hpp"
#include "nlohmann/json.hpp"
#include "stc/minilog.hpp"

#include <hazel/server/Hazel.hpp>
#include <hazel/meta/Macros.hpp>

#include <stc/StringUtil.hpp>
#include <cpr/cpr.h>

#include <openssl/evp.h>

void hazel::InitMinifluxProxy(HazelCore& server) {
    server.getApp()->route<"/api/miniflux/webhook", magpie::Method::Post>(
        &hazel::minifluxForwardToProxy
    );
}

void hazel::minifluxForwardToProxy(Context* ctx, magpie::Request& req, magpie::Response& res) {
    auto auth = req.headers["authorization"];
    if (auth.empty() || !auth.starts_with("Basic ")) {
        res = magpie::Response(
            magpie::Status::BadRequest,
            R"({"message": "Malformed or missing authorization header"})",
            "application/json"
        );
        return;
    }

    auto b64 = auth.substr(auth.find(' ') + 1);

    if (b64.size() % 4 != 0) {
        res = magpie::Response(
            magpie::Status::BadRequest,
            R"({"message": "Malformed or missing authorization header. Did you correctly base64-encode?"})",
            "application/json"
        );
        return;
    }

    // "For every 4 input bytes exactly 3 output bytes will be produced"
    // Hence the magic 3 / 4 division.
    // b64 is guaranteed to be divisible by 4 as well
    // ... but it looks like it's a bit wonky with null bytes, and occasionally add padding or something?
    const auto expectedSourceSize = (int) (3 * b64.size() / 4);
    std::basic_string<unsigned char> output(expectedSourceSize, '\0');
    const auto actualLength = EVP_DecodeBlock(
        output.data(),
        reinterpret_cast<const unsigned char *>(b64.c_str()),
        (int) b64.size()
    );

    std::string sOut(
        output.begin(),
        output.end()
    );
    // Strip null bytes
    sOut.erase(std::find(sOut.begin(), sOut.end(), '\0'), sOut.end());

    auto split = stc::string::split(sOut, ":", 1);
    if (expectedSourceSize != actualLength || split.size() != 2) {
        minilog::warn("{} from {} is a sus payload", auth, req.ipAddr);
        res = magpie::Response(
            magpie::Status::BadRequest,
            R"({"message": "Malformed or missing authorization header"})",
            "application/json"
        );
        return;
    }

    std::string username = split.at(0);
    std::string password = split.at(1);

    const auto& conf = ctx->conf;
    auto proxyIt = conf.getMinifluxProxies().find(username);
    if (proxyIt == conf.getMinifluxProxies().end() || proxyIt->second.passphrase != password) {
        minilog::info(
            "{} failed to log into {} (exists: {})",
            req.ipAddr,
            username, 
            proxyIt != conf.getMinifluxProxies().end()
        );
        minilog::info("{}", password);

        res = magpie::Response(
            magpie::Status::NotFound,
            R"({"message": "Webhook not found"})",
            "application/json"
        );
        return;
    }

    // TODO error handling
    nlohmann::json data = nlohmann::json::parse(req.body);
    const auto& proxy = proxyIt->second;

    //spdlog::info("{}", data.dump());
    auto evType = data.at("event_type").get<std::string>();
    auto adapter = conf.getAdapters().at(proxy.adapter);
    auto adapterConfig = proxyIt->second.adapter_config.value_or(nlohmann::json{});


    if (proxyIt->second.isEventEnabled(evType)) {
        minilog::info("Forwarding Miniflux webhook to {} (IP: {})", username, req.ipAddr);

        if (data.contains("entry")) {
            auto entry = data.at("entry");
            auto ce = detail::extractMinifluxEntry(entry);

            adapter->execute(ce.content, ce, adapterConfig);

        } else if (data.contains("entries")) {
            for (auto& entry : data.at("entries")) {
                auto ce = detail::extractMinifluxEntry(entry);
                adapter->execute(ce.content, ce, adapterConfig);
            }

        } else {
            minilog::info("No fucking clue how to parse {}", data.dump(4));

            res = magpie::Response(
                magpie::Status::InternalServerError,
                R"({"message": "Not sure how to parse miniflux data"})",
                "application/json"
            );
            return;
        }
    }
    // If the event isn't enabled, return 200 anyway. There's no point in sending an error to the server
    // when there isn't an error.

    res = magpie::Response(
        magpie::Status::OK,
        R"({"message": "OK"})",
        "application/json"
    );
}


hazel::detail::AdapterContent hazel::detail::extractMinifluxEntry(const nlohmann::json& entry) {
    AdapterContent c;

    c.content = entry.at("url");
    c.callbackUrl = c.content;

    return c;
}

