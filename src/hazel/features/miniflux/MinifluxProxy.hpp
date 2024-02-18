#pragma once

#include "cpr/response.h"
#include "hazel/server/Config.hpp"
#include "nlohmann/json_fwd.hpp"
#include <crow.h>
#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore;

extern void InitMinifluxProxy(HazelCore& server);

extern void minifluxForwardToProxy(HazelCore& server, crow::request& req, crow::response& res);


}
