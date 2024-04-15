#include "DashboardUpdaters.hpp"
#include "cpr/ssl_options.h"
#include "hazel/data/DashboardStructs.hpp"
#include <limits>
#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

namespace hazel {

bool Updaters::updateDashboardData(DashboardData &data, long long& waitSecs) {
    waitSecs = std::numeric_limits<long long>::max();

    return updateLinks(data, waitSecs);
}

bool Updaters::updateLinks(DashboardData& data, long long& waitSecs) {
    bool dirty = false;
    auto& links = *data.links;
    for (auto& link : links) {
        if (link.appType != LinkDynamicApp::_None) {
            auto now = Clock::now();
            if (link.lastUpdate + link.updateFrequency >= now) {
                // The update interval is not yet hit. Continue, and compute the wait
                long long diff = std::chrono::duration_cast<std::chrono::seconds>(
                    now - link.lastUpdate - link.updateFrequency
                ).count();
                waitSecs = std::min(waitSecs, diff);
                continue;
            }
            dirty = true;
                spdlog::info("here");

            // Update time. Compute waitSecsc as the min wait time
            waitSecs = std::min(waitSecs, (long long) link.updateFrequency.count());
            link.lastUpdate = now;
            // Run the update. Updates are delegated to various other functions
            Links::updaters.at(link.appType)(link);

        }
    }
    return dirty;
}

void Updaters::Links::updatePihole(DashboardLinkModule& link) {
    auto data = cpr::Get(
        cpr::Url{
            link.url + "/admin/api.php"
        },
        cpr::Parameters{
            {"summaryRaw", ""},
            {"auth", link.extras.at("api_key").get<std::string>()}
        },
        cpr::VerifySsl(link.extras.value("verify_ssl", true))
    );
    if (data.status_code >= 400 || data.status_code < 100) {
        spdlog::error("Bad response from Pi-hole (status_code = {}): {}", data.status_code, data.text);
        return;
    }

    auto json = nlohmann::json::parse(data.text);

    auto& fields = link.fields;
    fields["Queries today"] = std::to_string(json.at("dns_queries_today").get<long long>());
    fields["Blocked today"] = std::to_string(json.at("ads_blocked_today").get<long long>());
    fields["Blocked percent"] = std::to_string(json.at("ads_percentage_today").get<long long>());
}
void Updaters::Links::updateMiniflux(DashboardLinkModule& link) {
}
void Updaters::Links::updateUptimeKuma(DashboardLinkModule& link) {
}

}
