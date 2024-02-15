#include "WebCore.hpp"

#include <hazel/server/Hazel.hpp>
#include <hazel/meta/Macros.hpp>

void hazel::InitWebCore(HazelCore &server) {
    CROW_ROUTE(server.getApp(), "/_health")
        .methods(crow::HTTPMethod::GET)
        (HAZEL_CALLBACK_BINDING(webcoreHealth));
}

void hazel::webcoreHealth(HazelCore&, crow::request&, crow::response& res) {
    res.code = 200;
    res.end();
}
