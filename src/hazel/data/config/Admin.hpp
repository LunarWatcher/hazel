#pragma once

#include <string>
#include <map>

namespace hazel {

namespace Modules {

#define E2S(en) {#en, en}
#define S2E(en) {en, #en}
    enum Modules {
        AUTOMATION,
        LINKS
    };

    
    const static std::map<std::string, Modules> stringModuleMap {
        E2S(AUTOMATION),
        E2S(LINKS)
    };

    const static std::map<Modules, std::string> moduleStringMap {
        S2E(AUTOMATION),
        S2E(LINKS)
    };

#undef E2S
#undef S2E
}

struct EnabledModules {
    Modules::Modules moduleIdentifier;
    bool enabled;
};

struct GlobalAdminConfig {
    bool allowAccountCreation;

};

}
