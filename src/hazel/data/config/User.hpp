#pragma once

#include <string>

namespace hazel {

struct User {
    long long userId;

    std::string username;
    std::string password;

    bool isAdmin;

};

}
