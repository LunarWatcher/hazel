#pragma once

#include <hazel/automation/Adapter.hpp>

namespace hazel {

class DiscordAdapter : public Adapter {
private:
    std::string destUrl;

    std::string username;
    std::optional<std::string> avatarUrl;
public:
    DiscordAdapter(const nlohmann::json& config);

    virtual void execute(const std::string& content, const nlohmann::json& adapterConfig) override;
};

}
