#include "User.hpp"
#include "SQLiteCpp/Statement.h"

namespace hazel {


void User::load(SQLite::Statement& db) {
    this->userId = db.getColumn("ID");
    this->username = db.getColumn("Username").getString();
    this->password = db.getColumn("Password").getString();
    this->isAdmin = db.getColumn("IsAdmin").getInt() == 1;
}

SQLite::Statement User::store(SQLite::Database& db) {
    SQLite::Statement stmt(db,
        R"(
        INSERT OR REPLACE INTO Users (ID, Username, Password, IsAdmin)
        VALUES (?, ?, ?, ?)
        )"
    );
    stmt.bind(1, this->userId);
    stmt.bind(2, this->username);
    stmt.bind(3, this->password);
    stmt.bind(4, (int) isAdmin);

    return stmt;
}

}
