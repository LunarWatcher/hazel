#pragma once

#include "cpr/response.h"
#include "hazel/server/Config.hpp"
#include "nlohmann/json_fwd.hpp"
#include <crow.h>
#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore;

extern void InitWebCore(HazelCore& server);

extern void webcoreHealth(HazelCore& server, crow::request& req, crow::response& res);

}
