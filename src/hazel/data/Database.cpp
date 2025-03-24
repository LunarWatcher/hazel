#include "Database.hpp"
#include "SQLiteCpp/Database.h"

#include "db/Migration.hpp"

namespace hazel {

Database::Database(const std::filesystem::path& dbPath)
    : db(
        SQLite::Database(
            dbPath.string().c_str(),
            SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
        )
    )
{

}

void Database::initDatabase() {

    this->db.write([](auto& sdb) {
        Migration::prepMetatables(sdb);

        Migration m;
        m.pushVersion(R"(
            CREATE TABLE Users (
                ID INTEGER PRIMARY KEY AUTOINCREMENT,
                Username TEXT,
                Password TEXT,
                IsAdmin BOOLEAN
            );
            )");

        m.exec(sdb);

    });
}

}
