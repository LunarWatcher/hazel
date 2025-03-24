#include "Dashboard.hpp"

#include <hazel/server/Hazel.hpp>
#include <hazel/meta/Macros.hpp>

void hazel::InitDashboard(HazelCore &server) {
    HAZEL_REDIRECT("/", "/index.html");
    
    HAZEL_STATIC_PAGE("open-source.html", "Open-source licenses");

}


