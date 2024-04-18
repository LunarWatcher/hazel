#include "hazel/server/Hazel.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h"

int main() {
#ifdef HAZEL_DEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::cfg::load_env_levels();
#endif
    hazel::HazelCore::getInstance().init();
}
