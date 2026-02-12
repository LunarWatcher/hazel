#pragma once

#include "hazel/config/ConfigState.hpp"
#include <magpie/data/CommonData.hpp>

namespace hazel {

struct Context : public magpie::data::CommonData {
    ConfigState conf;

    Context(
        ConfigState&& conf
    ) :
        conf(std::move(conf))
    {}
};

}
