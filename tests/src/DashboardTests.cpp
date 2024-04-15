#include "hazel/data/DashboardStructs.hpp"
#include "hazel/server/Config.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Check base config support", "[Config]") {
    std::string config = R"(
    {
        "dashboard": {
            "links": [
                {
                    "name": "Pihole",
                    "type": "pihole",
                    "url": "https://127.0.0.1",
                    "config": {
                        "api_key": "abcd69420",
                        "update_frequency": 600
                    }
                },
                {
                    "name": "Pihole 2",
                    "url": "https://127.0.0.1"
                }
            ]
        }
    }
    )";
    auto conf = nlohmann::json::parse(config).get<hazel::Config>();

    REQUIRE(conf.adapters.size() == 0);
    REQUIRE(conf.miniflux_proxies.size() == 0);
    REQUIRE(conf.dashboard.links.size() == 2);
    {
        auto link = conf.dashboard.links.at(0);
        REQUIRE(link.config.has_value());
        REQUIRE(link.config->size() == 2);
        REQUIRE(link.name == "Pihole");
        REQUIRE(link.type == hazel::LinkDynamicApp::Pihole);
        REQUIRE(link.url == "https://127.0.0.1");
    }
    {
        auto link = conf.dashboard.links.at(1);
        REQUIRE_FALSE(link.config.has_value());
        REQUIRE(link.name == "Pihole 2");
        REQUIRE(link.type == hazel::LinkDynamicApp::_None);
        REQUIRE(link.url == "https://127.0.0.1");
    }
}
