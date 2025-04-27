#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <optional>

namespace hazel {

class Adapter {
public:

    struct Extras {
        std::optional<std::string> callbackUrl;
    };

    Adapter() = default;
    virtual ~Adapter() = default;

    virtual void execute(const std::string& content, const Extras& extras, const nlohmann::json& adapterConfig) = 0;
};

}
