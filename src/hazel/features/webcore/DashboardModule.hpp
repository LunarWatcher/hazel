#pragma once

#include "crow/mustache.h"
namespace hazel {

class DashboardModule {
public:
    virtual ~DashboardModule() = default;
    virtual void makeEndpoints() = 0;

    virtual void render(crow::mustache::context& ctx) = 0;

    virtual bool isConfigAdminOnly() = 0;
    virtual bool isModulePublic() = 0;
};

}
