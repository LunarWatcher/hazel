#pragma once

#include "SQLiteCpp/Database.h"
#include "hazel/sync/RWContainer.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <filesystem>
#include <shared_mutex>

namespace hazel {

class Database {
private:
    RWContainer<SQLite::Database> db;

public:
    Database(const std::filesystem::path& dbPath);
    void initDatabase();

    RWContainer<SQLite::Database>* operator->() {
        return &db;
    }

};

}
