#pragma once

#include <crow.h>
#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore;

extern void InitWebCore(HazelCore& server);

extern void webcoreHealth(HazelCore& server, crow::request& req, crow::response& res);

}
