#pragma once

#include "hazel/automation/Adapter.hpp"
#include "hazel/config/CoreConfig.hpp"
#include "hazel/config/modules/MinifluxConfig.hpp"
#include <string>
namespace hazel {

class ConfigState {
private:
    std::string confPath;
    CoreConfig core;

    std::map<std::string, MinifluxProxy> minifluxProxies;

    std::map<std::string, std::shared_ptr<Adapter>> adapters;

public:
    ConfigState(const std::string& path);

    const CoreConfig& getCoreConfig() const { return core; }
    const decltype(minifluxProxies)& getMinifluxProxies() const { return minifluxProxies; }
    const decltype(adapters)& getAdapters() const { return adapters; }
};

}
