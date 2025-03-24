#pragma once

#include "hazel/config/meta/Loadable.hpp"
#include <string>

namespace hazel {

struct User : public Loadable {
    int64_t userId;

    std::string username;
    std::string password;

    bool isAdmin;

    void load(SQLite::Statement& db) override;
    SQLite::Statement store(SQLite::Database& db) override;

};

}
