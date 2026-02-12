#include "Hazel.hpp"
#include "hazel/features/miniflux/MinifluxProxy.hpp"
#include "hazel/middlewares/BaseMiddlewares.hpp"
#include "magpie/config/AppConfig.hpp"

#include <stc/minilog.hpp>

namespace hazel {

HazelCore::HazelCore(const std::string& configRoot)  {
    context = std::make_shared<Context>(
        ConfigState { configRoot }
    );

    this->app = std::make_shared<magpie::App<Context>>(context, magpie::AppConfig {
        .port = 1337,
        .ssl = magpie::SSLConfig::fromGeneratedCertificate()
    });
    this->app->registerGlobalMiddlewares({
        std::make_shared<SecurityMetaHeaders>(),
    });
}

void HazelCore::bootstrapDatabase() {
    context->db.initDatabase();
}

void HazelCore::prepareHandlers() {
    minilog::info("Loading endpoints...");

    InitMinifluxProxy(*this);
}

void HazelCore::run(bool test) {
    bootstrapDatabase();
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
