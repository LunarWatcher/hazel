#include "Dashboard.hpp"
#include "crow/app.h"
#include "crow/common.h"
#include "crow/mustache.h"
#include "crow/websocket.h"

#include <hazel/server/Hazel.hpp>
#include <hazel/meta/Macros.hpp>

void hazel::InitDashboard(HazelCore &server) {
    HAZEL_REDIRECT("/", "/index.html");
    CROW_ROUTE(server.getApp(), "/index.html")
        .methods(crow::HTTPMethod::GET)
        (HAZEL_CALLBACK_BINDING(webcoreIndex));

    HAZEL_STATIC_ASSET("style.css");
    HAZEL_STATIC_ASSET("base.js");
    
    HAZEL_STATIC_PAGE("open-source.html", "Open-source licenses");

    // TODO Crow websockets have problems: https://github.com/CrowCpp/Crow/issues/506
    // This can result in problems down the line, so might need to spend some time writing a
    // fix for that issue too
    CROW_WEBSOCKET_ROUTE(server.getApp(), "/api/sockets/dashboard")
        .onaccept([&](const crow::request& req, void** userdata){
            
            return true;
        })
        .onmessage([](crow::websocket::connection& conn, const std::string& data, bool isBinary) {
            if (isBinary) {
                return;
            }
            nlohmann::json j;
            try {
                j = nlohmann::json::parse(data);
            } catch (nlohmann::json::exception& e) {
                conn.send_text(R"({"error": "All websocket messages must be JSON. See the docs for the spec"})");
                return;
            }

            if (!j.contains("command")) {
                conn.send_text(R"({"error": "Malformed request JSON"})");
                return;
            }
            auto commandJson = j.at("command");
            if (!commandJson.is_string()) {
                conn.send_text(R"({"error": "Malformed request JSON"})");
                return;
            }

            auto command = commandJson.get<std::string>();
            if (command == "refresh") {
                conn.send_text("Data goes here :=)");
            } else {
                conn.send_text(R"({"error": "Unrecognised command"})");
            }
            
        });
}

void hazel::webcoreIndex(HazelCore &server, crow::request &req, crow::response &res) {
    crow::mustache::context ctx;

    HAZEL_WEBPAGE(res, ctx, "index.mustache", "Dashboard");

    res.end();
}
