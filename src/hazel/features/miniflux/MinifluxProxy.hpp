#pragma once

#include "hazel/automation/Adapter.hpp"
#include <crow.h>
#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore;

extern void InitMinifluxProxy(HazelCore& server);

extern void minifluxForwardToProxy(HazelCore& server, crow::request& req, crow::response& res);

namespace detail {

struct AdapterContent : public Adapter::Extras {
    std::string content;
};

extern AdapterContent extractMinifluxEntry(const nlohmann::json& entry);

}


}
