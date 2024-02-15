#pragma once

#include "crow/http_request.h"
#include "crow/http_response.h"
#include "crow/middleware.h"

namespace hazel {

struct SecurityMetaHeaders {
    struct context {
    };
    void before_handle(crow::request&, crow::response&, context&) {}
    void after_handle(crow::request& req, crow::response& res, context&);
};

}
