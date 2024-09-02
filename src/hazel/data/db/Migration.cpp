#include "Migration.hpp"
#include "SQLiteCpp/Statement.h"
#include "SQLiteCpp/Transaction.h"

#include <hazel/data/Database.hpp>

#include <spdlog/spdlog.h>
#include <stdexcept>

namespace hazel {

Migration& Migration::pushVersion(const std::string &query) {
    this->queries.push_back(query);

    return *this;
}

void Migration::exec(SQLite::Database& db) {
    SQLite::Transaction tx(db);

    SQLite::Statement stmt(db, R"(
        SELECT Version FROM Migrations WHERE Name = 'Version'
    )");

    int64_t headVersion = 0;
    if (stmt.executeStep()) {
        headVersion = stmt.getColumn(0).getInt64();
    }

    if (headVersion < 0) {
        spdlog::error(
            "The Migrations table has been tampered with. Cannot resolve the current database state. "
            "If you've fucked around with the Migrations table, first, why? Second, this is not a bug. "
            "You have to purge your entire database. If you haven't, please open a bug report. This should not normally "
            "happen, unless you've done something weird to the database."
        );
        tx.rollback();
        throw std::runtime_error("Database fucked");
    }

    if (headVersion < (int64_t) this->queries.size() + versionOffset) {
        spdlog::info("Database out of date (current version: {}, new version: {})", headVersion, queries.size());
        for (size_t i = headVersion - versionOffset; i < queries.size(); ++i) {
            spdlog::info("Updating to v{}", i + 1 + versionOffset);
            db.exec(queries.at(i + versionOffset));
        }

        SQLite::Statement stmt(db, R"(
        INSERT OR REPLACE INTO Migrations (Name, Version)
        VALUES
            ('Version', ?)
        )");
        stmt.bind(1, int64_t(queries.size()) + versionOffset);

        if (stmt.exec() == 0) {
            tx.rollback();
            spdlog::error("Failed to upgrade database");
            throw std::runtime_error("Migration error");
        }

        spdlog::info("Update successful.");

    } else {
        spdlog::info("Database up-to-date (version: {})", headVersion);
    }



    tx.commit();
}

void Migration::prepMetatables(SQLite::Database& db) {
    spdlog::info("Initialising migration table...");
    SQLite::Transaction tx(db);
    db.exec(R"(
    CREATE TABLE IF NOT EXISTS Migrations (
        Name TEXT PRIMARY KEY,
        Version INTEGER NOT NULL
    );
    )");
    tx.commit();
}

}
