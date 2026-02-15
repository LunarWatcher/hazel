#include "Hazel.hpp"
#include "hazel/features/miniflux/MinifluxProxy.hpp"
#include "hazel/middlewares/BaseMiddlewares.hpp"
#include "magpie/config/AppConfig.hpp"
#include "magpie/logger/Logger.hpp"

#include <stc/minilog.hpp>

namespace hazel {

HazelCore::HazelCore(const std::string& configRoot)  {
    context = std::make_shared<Context>(
        ConfigState { configRoot }
    );

    this->app = std::make_shared<magpie::App<Context>>(context, magpie::AppConfig {
        .port = context->conf.getCoreConfig().port,
        // .ssl = magpie::SSLConfig::fromGeneratedCertificate()
    });
    this->app->registerGlobalMiddlewares({
        std::make_shared<SecurityMetaHeaders>(),
    });
    minilog::config().level = minilog::Level::Debug;
    magpie::logger::config().logger = [](auto level, const auto& msg) {
        switch (level) {
            case magpie::logger::Level::debug:
                minilog::debug("{}", msg);
                break;
            case magpie::logger::Level::info:
                minilog::info("{}", msg);
                break;
            case magpie::logger::Level::warning:
                minilog::warn("{}", msg);
                break;
            case magpie::logger::Level::error:
                minilog::error("{}", msg);
                break;
            case magpie::logger::Level::critical:
                minilog::critical("{}", msg);
                break;
        }
    };
}

void HazelCore::prepareHandlers() {
    minilog::info("Loading endpoints...");

    InitMinifluxProxy(*this);
}

void HazelCore::run(bool test) {
    prepareHandlers();

    if (!test) {
        app->run();
    }
}

void HazelCore::init() {
    HazelCore& p = HazelCore::getInstance();

    p.run(false);
}

}
