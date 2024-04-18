#pragma once

#include "hazel/meta/Typedefs.hpp"
#include <hazel/json/SerialiserImpl.hpp>
#include <nlohmann/json.hpp>

namespace hazel {


enum class LinkDynamicApp {
    Miniflux,
    UptimeKuma,
    Pihole,
    /**
     * Used to represent a link that _isn't_ dynamic.
     */
    _None
};
NLOHMANN_JSON_SERIALIZE_ENUM(
    LinkDynamicApp,
    {
        {LinkDynamicApp::Miniflux, "miniflux"},
        {LinkDynamicApp::Pihole, "pihole"},
        {LinkDynamicApp::UptimeKuma, "uptime-kuma"},
        {LinkDynamicApp::_None, "__UNDEFINED__"},
    }
);

struct DashboardLinkModule {
    // Exported fields {{{
    std::string title;
    std::string url;
    std::string icon;
    std::string colour;

    /**
     * Describes extra fields to display under the service card.
     * This is only populated for dynamic apps
     */
    std::map<std::string, std::string> fields;
    // }}}
    // Internal fields {{{
    Clock::time_point lastUpdate;
    std::chrono::seconds updateFrequency;

    LinkDynamicApp appType;
    /**
     * Extra config fields. Only used if appType != _None
     * Implementation-dependent and not exported to the client, as
     * this may contain API tokens.
     */
    nlohmann::json extras;
    // }}}

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        DashboardLinkModule,
        title,
        url,
        icon,
        colour,
        fields
    );
};

struct DashboardData {
    std::optional<std::vector<DashboardLinkModule>> links;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        DashboardData,
        links
    );
};

}
