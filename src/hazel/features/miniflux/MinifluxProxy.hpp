#pragma once

#include "hazel/automation/Adapter.hpp"
#include "hazel/server/Context.hpp"
#include "magpie/transfer/Request.hpp"
#include "magpie/transfer/Response.hpp"
#include <nlohmann/json.hpp>

namespace hazel {

class HazelCore;

extern void InitMinifluxProxy(HazelCore& server);
extern void minifluxForwardToProxy(Context*, magpie::Request& req, magpie::Response& res);

namespace detail {

struct AdapterContent : public Adapter::Extras {
    std::string content;
};

extern AdapterContent extractMinifluxEntry(const nlohmann::json& entry);

}


}
