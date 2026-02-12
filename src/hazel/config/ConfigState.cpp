#include "ConfigState.hpp"
#include "hazel/automation/adapters/DiscordAdapter.hpp"
#include "hazel/automation/adapters/NtfyAdapter.hpp"
#include "nlohmann/json.hpp"
#include "stc/minilog.hpp"
#include <fstream>
#include <stdexcept>

namespace hazel {

ConfigState::ConfigState(const std::string& path) : confPath(path) {

    std::ifstream ifs(path);
    if (!ifs) {
        minilog::warn("No hazel.conf; internal defaults will be used");
    } else {
        minilog::debug("Found {}", path);
        nlohmann::json conf;
        ifs >> conf;

        if (conf.contains("core")) {
            core = conf.at("core");
        } 
        if (conf.contains("miniflux-proxies")) {
            minifluxProxies = conf.at("miniflux-proxies");
            minilog::debug("Found miniflux-proxies with {} values", minifluxProxies.size());
        }
        if (conf.contains("adapters")) {
            minilog::debug("Found adapters");

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

    ensurePathsExist();

}

void ConfigState::ensurePathsExist() {

    std::filesystem::create_directories(core.dataRoot);

}

}
