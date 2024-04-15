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
    CROW_ROUTE(server.getApp(), "/api/dashboard")
        .methods(crow::HTTPMethod::GET)
        (HAZEL_CALLBACK_BINDING(webcoreDashboardData));

    HAZEL_STATIC_ASSET("style.css");
    HAZEL_STATIC_ASSET("base.js");
    
    HAZEL_STATIC_PAGE("open-source.html", "Open-source licenses");

}

void hazel::webcoreIndex(HazelCore &, crow::request &, crow::response &res) {
    crow::mustache::context ctx;

    HAZEL_WEBPAGE(res, ctx, "index.mustache", "Dashboard");

    res.end();
}

void hazel::webcoreDashboardData(HazelCore& server, crow::request&, crow::response& res) {
    HAZEL_JSON(res);
    res.end(server.getDashData().getJsonData());
}
