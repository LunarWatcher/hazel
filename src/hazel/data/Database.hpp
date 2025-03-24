#pragma once

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include "hazel/config/meta/Loadable.hpp"
#include "hazel/sync/RWContainer.hpp"
#include <SQLiteCpp/SQLiteCpp.h>
#include <filesystem>

namespace hazel {

class Database {
private:
    RWContainer<SQLite::Database> db;

    template <
        typename T,
        typename std::enable_if<std::is_base_of_v<Loadable, T>>
    > 
    T load(SQLite::Statement& stmt) {
        T obj;
        obj.load(stmt);
        return obj;
    }

    template <
        typename T,
        typename std::enable_if<std::is_base_of_v<Loadable, T>>
    > 
    SQLite::Statement store(SQLite::Database& db, T& obj) {
        return obj.store(db);
    }
public:
    Database(const std::filesystem::path& dbPath);
    void initDatabase();

    RWContainer<SQLite::Database>* operator->() {
        return &db;
    }



};

}
