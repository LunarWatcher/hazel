#pragma once

#include "hazel/automation/Adapter.hpp"
namespace hazel {

class NtfyAdapter : public Adapter {
private:
    std::string destUrl;
    std::string defaultTopic;
    std::string authHeader;
public:
    NtfyAdapter(const nlohmann::json& config);

    virtual void execute(const std::string& content, const nlohmann::json& adapterConfig) override;

};

}
