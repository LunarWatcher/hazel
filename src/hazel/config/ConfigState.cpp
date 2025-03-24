#include "ConfigState.hpp"
#include "hazel/automation/adapters/DiscordAdapter.hpp"
#include "hazel/automation/adapters/NtfyAdapter.hpp"
#include "nlohmann/json.hpp"
#include "spdlog/spdlog.h"
#include <fstream>
#include <fmt/format.h>
#include <stdexcept>

namespace hazel {

ConfigState::ConfigState(const std::string& path) : confPath(path) {

    std::ifstream ifs(path);
    if (!ifs) {
        spdlog::warn("No hazel.conf; internal defaults will be used");
    } else {
        spdlog::debug("Found {}", path);
        nlohmann::json conf;
        ifs >> conf;

        if (conf.contains("core")) {
        } 
        if (conf.contains("miniflux-proxies")) {
            minifluxProxies = conf.at("miniflux-proxies");
            spdlog::debug("Found miniflux-proxies with {} values", minifluxProxies.size());
        }
        if (conf.contains("adapters")) {
            spdlog::debug("Found adapters");

            for (const auto& [name, adapterConf] : conf.at("adapters").items()) {
                auto type = adapterConf.at("type");

                std::shared_ptr<Adapter> ptr;
                if (type == "ntfy") {
                    ptr = std::make_shared<NtfyAdapter>(
                        adapterConf
                    );
                } else if (type == "discord") {
                    ptr = std::make_shared<DiscordAdapter>(
                        adapterConf
                    );
                } else {
                    throw std::runtime_error("Config error: invalid adapter type");
                }

                adapters[name] = ptr;

            }
        }
    }


}

}
