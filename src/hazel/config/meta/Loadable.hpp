#pragma once

#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"

namespace hazel {

/**
 * Describes a struct stored in SQL.
 *
 * Note that it's up to the object itself to determine how it's stored and loaded.
 * Migrations are handled through primarily manual systems in the database, and only
 * used if a ✨ unique ✨ table is used for a given struct.
 *
 * There's no constraint requiring one struct to be one table, though this can be common
 * for many applications.
 */
struct Loadable {
    virtual void load(SQLite::Statement& stmt) = 0;
    virtual SQLite::Statement store(SQLite::Database& db) = 0;
};

}
