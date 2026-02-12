#pragma once

#include "hazel/server/Context.hpp"
#include <magpie/middlewares/Middleware.hpp>

namespace hazel {

class SecurityMetaHeaders : public magpie::Middleware<Context> {
public:
    void onRequest(
        magpie::IMiddlewareProcessor<Context> *proc,
        Context* ctx,
        magpie::Request& req,
        magpie::Response& res
    ) override;
};

}
