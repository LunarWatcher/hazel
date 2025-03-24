#pragma once

#include <crow.h>
#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore;

extern void InitDashboard(HazelCore& server);

}
