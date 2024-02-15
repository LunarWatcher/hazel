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

#define HAZEL_STATIC_ASSET(filename) CROW_ROUTE(server.getApp(), "/assets/" filename)([&](crow::response& res) { \
    res.set_static_file_info(server.getAssetBaseDir() + ("/assets/" filename)); \
    res.end(); \
})

