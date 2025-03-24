#pragma once

#include <crow.h>
#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore;

extern void InitMinifluxProxy(HazelCore& server);

extern void minifluxForwardToProxy(HazelCore& server, crow::request& req, crow::response& res);


}
