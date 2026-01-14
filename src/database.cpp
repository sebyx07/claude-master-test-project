#include "todolist/database.h"
#include <sqlite3.h>
#include <sstream>

namespace todolist {

Database::Database(const std::string& db_path)
    : db_(nullptr)
{
    int result = sqlite3_open(db_path.c_str(), &db_);

    if (result != SQLITE_OK) {
        std::string error_msg = "Failed to open database: ";
        if (db_) {
            error_msg += sqlite3_errmsg(db_);
            sqlite3_close(db_);
        } else {
            error_msg += "unable to allocate memory";
        }
        db_ = nullptr;
        throw DatabaseException(error_msg);
    }

    // Initialize schema (create tables if needed)
    try {
        initializeSchema();
    } catch (...) {
        sqlite3_close(db_);
        db_ = nullptr;
        throw;
    }
}

Database::~Database() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

Database::Database(Database&& other) noexcept
    : db_(other.db_)
{
    other.db_ = nullptr;
}

Database& Database::operator=(Database&& other) noexcept {
    if (this != &other) {
        if (db_) {
            sqlite3_close(db_);
        }
        db_ = other.db_;
        other.db_ = nullptr;
    }
    return *this;
}

void Database::execute(const std::string& sql) {
    if (!db_) {
        throw DatabaseException("Database is not open");
    }

    char* error_msg = nullptr;
    int result = sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &error_msg);

    if (result != SQLITE_OK) {
        std::string error = "SQL execution failed: ";
        if (error_msg) {
            error += error_msg;
            sqlite3_free(error_msg);
        } else {
            error += sqlite3_errmsg(db_);
        }
        throw DatabaseException(error);
    }
}

std::string Database::getLastError() const {
    if (!db_) {
        return "Database is not open";
    }
    return sqlite3_errmsg(db_);
}

void Database::initializeSchema() {
    const char* create_table_sql = R"(
        CREATE TABLE IF NOT EXISTS todos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            description TEXT,
            completed INTEGER DEFAULT 0,
            created_at INTEGER NOT NULL
        );
    )";

    execute(create_table_sql);

    // Create an index on completed status for faster filtering
    const char* create_index_sql = R"(
        CREATE INDEX IF NOT EXISTS idx_todos_completed
        ON todos(completed);
    )";

    execute(create_index_sql);
}

} // namespace todolist
