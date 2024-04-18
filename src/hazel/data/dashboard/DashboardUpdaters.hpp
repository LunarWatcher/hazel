#pragma once

#include <functional>
#include "hazel/data/DashboardStructs.hpp"
#include <map>

namespace hazel {

namespace Updaters {

extern bool updateDashboardData(DashboardData& data, long long& waitSecs);

extern bool updateLinks(DashboardData& data, long long& waitSecs);

namespace Links {

extern void updatePihole(DashboardLinkModule& link);
extern void updateMiniflux(DashboardLinkModule& link);
extern void updateUptimeKuma(DashboardLinkModule& link);

inline std::map<LinkDynamicApp, std::function<void(DashboardLinkModule&)>> updaters = {
    {LinkDynamicApp::Miniflux, updateMiniflux},
    {LinkDynamicApp::Pihole, updatePihole},
    {LinkDynamicApp::UptimeKuma, updateUptimeKuma},
};

}
}

}
