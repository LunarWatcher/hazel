#pragma once

#include "nlohmann/detail/macro_scope.hpp"
#include <hazel/json/SerialiserImpl.hpp>
#include <nlohmann/json.hpp>

namespace hazel {


enum class LinkDynamicApp {
    Miniflux,
    UptimeKuma,
    Pihole
};
NLOHMANN_JSON_SERIALIZE_ENUM(
    LinkDynamicApp,
    {
        {LinkDynamicApp::Miniflux, "miniflux"},
        {LinkDynamicApp::Pihole, "pihole"},
        {LinkDynamicApp::UptimeKuma, "uptime-kuma"},
    }
);

struct DashboardLinkModule {
    std::string title;
    std::string url;
    std::string icon;
    std::string colour;

    std::map<std::string, std::string> fields;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        DashboardLinkModule,
        title,
        url,
        fields
    );
};

}
