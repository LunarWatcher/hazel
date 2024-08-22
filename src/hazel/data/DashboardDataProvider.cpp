#include "DashboardDataProvider.hpp"
#include "hazel/data/DashboardStructs.hpp"
#include "hazel/data/dashboard/DashboardUpdaters.hpp"
#include "hazel/server/Config.hpp"
#include "spdlog/spdlog.h"
#include <limits>

namespace hazel {

DashboardDataProvider::DashboardDataProvider(DashboardConfig& conf) {
    if (!conf.links.empty()) {
        std::vector<DashboardLinkModule> links;
        for (auto& link : conf.links) {
            long long updateFreq = 60;
            if (link.config.has_value()
                && link.config->contains("update_frequency")) {
                updateFreq = link.config->at("update_frequency").get<long long>();
                if (updateFreq < 5) {
                    updateFreq = 5;
                }
            } else {
                switch (link.type) {
                case LinkDynamicApp::Pihole:
                    updateFreq = 600;
                    break;
                case LinkDynamicApp::Miniflux:
                    updateFreq = 1800;
                    break;
                default:;
                }
            }
            links.push_back(DashboardLinkModule {
                .title = link.name,
                .url = link.url,
                .icon = "TODO",
                .colour = "#000",
                .fields = {},
                .lastUpdate = Clock::time_point(std::chrono::seconds(0)),
                .updateFrequency = std::chrono::seconds(updateFreq),
                .appType = link.type,
                .extras = link.config.value_or(nlohmann::json::object()),
            });
            
        }
        this->dataArchive.links = std::move(links);
    }

    this->updateProcessor = std::thread(std::bind(&DashboardDataProvider::update, this));
}

void DashboardDataProvider::update() {
    while (true) {
        // Dirty is set to cache.empty() to make sure the string is initialised properly at least once.
        // This can be necessary if, after the initialisation, none of the dashboard components 
        // contain dynamic data. This would result in the update function returning false for all
        // components, resulting in an empty string.
        //
        // Also note that this raw access is safe. The update function is the only function that does a write,
        // and locking read-only is unnecessary here.
        // Even with a write on another thread, length should never dip back to zero even in a race
        // condition, so it should be fine. 
        // Plus, if multi-thread writes become a thing, the data archive also needs to be
        // wrapped in a thread-safe wrapper, which I foresee could be Fun:tm:. This should
        // never work with enough data for that to be applicable though, so I'm choosing to
        // completely ignore it :p
        long long sleepTime;
        bool dirty = 
            Updaters::updateDashboardData(dataArchive, sleepTime)
            || jsonCache.raw().empty();

        // In some cases, the next update for a Thing:tm: can be scheduled immediately after the last update.
        // This max statement is to make sure it isn't immediately or almost immediately executed to reduce cache writes.
        sleepTime = std::max(sleepTime, 5ll);

        if (dirty) {
            this->jsonCache.write([&](std::string& cache) {
                cache = nlohmann::json(dataArchive).dump();
            });
        }
        if (sleepTime == std::numeric_limits<long long>::max()) {
            spdlog::warn("sleepTime is the max long value. The dashboard probably doesn't have anything in it that needs updates. Stopping update thread. If this is wrong, please open a bug report: https://github.com/LunarWatcher/hazel/issues/");
            break;
        }
        spdlog::debug("Dashboard data refreshed (dirty = {}). Sleeping for {}", dirty, sleepTime);
        std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
    } 
}

std::string DashboardDataProvider::getJsonData() {
    return this->jsonCache.copy();
}

}
