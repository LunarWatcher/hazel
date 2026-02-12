#pragma once

#include "hazel/config/ConfigState.hpp"
#include "hazel/data/Database.hpp"
#include <magpie/data/CommonData.hpp>

namespace hazel {

struct Context : public magpie::data::CommonData {
    ConfigState conf;
    Database db;

    Context(
        ConfigState&& conf
    ) :
        conf(std::move(conf)),
        db(
            std::filesystem::path(
                this->conf.getCoreConfig().dataRoot
            ) / "hazel.sqlite3"
        ) {}
};

}
