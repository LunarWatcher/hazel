#pragma once

#include "crow.h"

#include "crow/middlewares/cookie_parser.h"
#include "crow/middlewares/session.h"
#include <hazel/middlewares/BaseMiddlewares.hpp>

#define GET(type, thing, key, defaultValue) auto raw##key = thing.get(#key); \
     type key = raw##key == nullptr ? defaultValue : raw##key;

namespace hazel {

using Server = crow::Crow<
    SecurityMetaHeaders,
    crow::CookieParser
>;

}
