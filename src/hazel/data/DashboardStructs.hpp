#pragma once

#include <hazel/json/SerialiserImpl.hpp>
#include <nlohmann/json.hpp>

namespace hazel {

struct Dnf2bStats {
    int bannedClients;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Dnf2bStats, bannedClients);
};

struct LinkField {
    std::string title;
    std::string value;
    std::string displayType;
};

struct LinkProvider {
    std::string link;
    std::string name;
    std::vector<LinkField> fields;
};

struct LinkMap {
    std::map<std::string, LinkProvider> providers;
};

struct DashboardData {
    std::optional<Dnf2bStats> dnf2bStats;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        DashboardData,
        dnf2bStats
    );
};

}
