#include <functional>


/**
 * Helper macro for binding a callback
 */
#define HAZEL_CALLBACK_BINDING(funcName) std::bind(funcName, std::ref(server), std::placeholders::_1, std::placeholders::_2)

/**
 * Helper macro for binding a class-level callback
 */
#define HAZEL_CLASS_CALLBACK_BINDING(funcName) std::bind(funcName, this, std::ref(server), std::placeholders::_1, std::placeholders::_2)

#define HAZEL_JSON(field) field.add_header("Content-Type", "application/json");
#define HAZEL_HTML(field) field.add_header("Content-Type", "text/html");

#define HAZEL_STATIC_ASSET(filename) CROW_ROUTE(server.getApp(), "/assets/" filename)([&](crow::response& res) { \
    res.set_static_file_info(server.getAssetBaseDir() + ("/assets/" filename)); \
    res.end(); \
})

#define HAZEL_REDIRECT(from, to) CROW_ROUTE(server.getApp(), from)([&](crow::response& res) { \
    res.moved_perm(to); \
    res.end(); \
});


#define HAZEL_COMMON_CONTEXT(ctx) \
    ctx["Version"] = HAZEL_VERSION;

#define HAZEL_WEBPAGE(res, ctx, contentPartial, title) \
    ctx["Title"] = (std::string) title; \
    HAZEL_COMMON_CONTEXT(ctx); \
    /* Why the actual fuck is a capture required to make this work? */ \
    ctx["Content"] = [&](std::string&) { \
        return std::string("{{>" contentPartial "}}"); \
    }; \
    res.body = crow::mustache::load("partials/base.mustache").render_string(ctx); \
    HAZEL_HTML(res);

#define HAZEL_STATIC_PAGE(filename, title) CROW_ROUTE(server.getApp(), "/" filename)([&](crow::response& res) { \
    crow::mustache::context ctx; \
    HAZEL_WEBPAGE(res, ctx, filename, title); \
    res.end(); \
})
