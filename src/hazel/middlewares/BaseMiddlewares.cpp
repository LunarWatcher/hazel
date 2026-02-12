#include "BaseMiddlewares.hpp"

namespace hazel {


void SecurityMetaHeaders::onRequest(
        magpie::IMiddlewareProcessor<Context> *proc,
        Context* ctx,
        magpie::Request& req,
        magpie::Response& res
) {
    next(proc, ctx, req, res);
    res.headers["Server"] = "LunarWatcher/Hazel " HAZEL_VERSION;
    res.headers["X-Frame-Options"] = "DENY";
    res.headers["X-Content-Type-Options"] = "nosniff";
    res.headers["Referrer-Policy"] = "strict-origin-when-cross-origin";
    res.headers["X-XSS-Protection"] = "0";
}

}
