#pragma once

#include <crow.h>
#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore;

extern void InitDashboard(HazelCore& server);
extern void webcoreIndex(HazelCore& server, crow::request& req, crow::response& res);

}
