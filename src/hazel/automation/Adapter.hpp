#pragma once

#include "cpr/cprtypes.h"
#include <nlohmann/json.hpp>
#include <string>

namespace hazel {

class Adapter {
public:
    Adapter() = default;
    virtual ~Adapter() = default;

    virtual void execute(const std::string& content, const nlohmann::json& adapterConfig) = 0;
};

}
