#include "todolist/todo_repository.h"
#include <sqlite3.h>
#include <sstream>

namespace todolist {

TodoRepository::TodoRepository(Database& database)
    : database_(database)
{
}

TodoItem TodoRepository::create(const TodoItem& item) {
    const char* sql = "INSERT INTO todos (title, description, completed, created_at) VALUES (?, ?, ?, ?)";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare insert statement: " + database_.getLastError());
    }

    // Bind parameters
    sqlite3_bind_text(stmt, 1, item.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, item.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, item.isCompleted() ? 1 : 0);
    sqlite3_bind_int64(stmt, 4, static_cast<sqlite3_int64>(item.getCreatedAtUnix()));

    // Execute
    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE) {
        std::string error = database_.getLastError();
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to insert todo item: " + error);
    }

    // Get the inserted id
    int id = static_cast<int>(sqlite3_last_insert_rowid(database_.getHandle()));
    sqlite3_finalize(stmt);

    // Return a copy with the id set
    TodoItem created_item = item;
    created_item.setId(id);
    return created_item;
}

std::optional<TodoItem> TodoRepository::findById(int id) {
    const char* sql = "SELECT id, title, description, completed, created_at FROM todos WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare select statement: " + database_.getLastError());
    }

    sqlite3_bind_int(stmt, 1, id);

    result = sqlite3_step(stmt);

    if (result == SQLITE_ROW) {
        TodoItem item = readTodoItem(stmt);
        sqlite3_finalize(stmt);
        return item;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

std::vector<TodoItem> TodoRepository::findAll() {
    const char* sql = "SELECT id, title, description, completed, created_at FROM todos ORDER BY created_at DESC";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare select statement: " + database_.getLastError());
    }

    std::vector<TodoItem> items;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        items.push_back(readTodoItem(stmt));
    }

    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        throw DatabaseException("Error reading todo items: " + database_.getLastError());
    }

    return items;
}

std::vector<TodoItem> TodoRepository::findCompleted() {
    const char* sql = "SELECT id, title, description, completed, created_at FROM todos WHERE completed = 1 ORDER BY created_at DESC";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare select statement: " + database_.getLastError());
    }

    std::vector<TodoItem> items;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        items.push_back(readTodoItem(stmt));
    }

    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        throw DatabaseException("Error reading completed items: " + database_.getLastError());
    }

    return items;
}

std::vector<TodoItem> TodoRepository::findPending() {
    const char* sql = "SELECT id, title, description, completed, created_at FROM todos WHERE completed = 0 ORDER BY created_at DESC";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare select statement: " + database_.getLastError());
    }

    std::vector<TodoItem> items;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        items.push_back(readTodoItem(stmt));
    }

    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        throw DatabaseException("Error reading pending items: " + database_.getLastError());
    }

    return items;
}

std::vector<TodoItem> TodoRepository::findByTitle(const std::string& query) {
    const char* sql = "SELECT id, title, description, completed, created_at FROM todos WHERE title LIKE ? ORDER BY created_at DESC";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare select statement: " + database_.getLastError());
    }

    // Add wildcards for partial matching
    std::string search_pattern = "%" + query + "%";
    sqlite3_bind_text(stmt, 1, search_pattern.c_str(), -1, SQLITE_TRANSIENT);

    std::vector<TodoItem> items;
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        items.push_back(readTodoItem(stmt));
    }

    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE) {
        throw DatabaseException("Error searching todo items: " + database_.getLastError());
    }

    return items;
}

bool TodoRepository::update(const TodoItem& item) {
    const char* sql = "UPDATE todos SET title = ?, description = ?, completed = ? WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare update statement: " + database_.getLastError());
    }

    sqlite3_bind_text(stmt, 1, item.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, item.getDescription().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, item.isCompleted() ? 1 : 0);
    sqlite3_bind_int(stmt, 4, item.getId());

    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE) {
        std::string error = database_.getLastError();
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to update todo item: " + error);
    }

    int changes = sqlite3_changes(database_.getHandle());
    sqlite3_finalize(stmt);

    return changes > 0;
}

bool TodoRepository::remove(int id) {
    const char* sql = "DELETE FROM todos WHERE id = ?";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare delete statement: " + database_.getLastError());
    }

    sqlite3_bind_int(stmt, 1, id);

    result = sqlite3_step(stmt);

    if (result != SQLITE_DONE) {
        std::string error = database_.getLastError();
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to delete todo item: " + error);
    }

    int changes = sqlite3_changes(database_.getHandle());
    sqlite3_finalize(stmt);

    return changes > 0;
}

int TodoRepository::count() {
    const char* sql = "SELECT COUNT(*) FROM todos";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare count statement: " + database_.getLastError());
    }

    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to count todo items: " + database_.getLastError());
    }

    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    return count;
}

int TodoRepository::countCompleted() {
    const char* sql = "SELECT COUNT(*) FROM todos WHERE completed = 1";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare count statement: " + database_.getLastError());
    }

    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to count completed items: " + database_.getLastError());
    }

    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    return count;
}

int TodoRepository::countPending() {
    const char* sql = "SELECT COUNT(*) FROM todos WHERE completed = 0";

    sqlite3_stmt* stmt = nullptr;
    int result = sqlite3_prepare_v2(database_.getHandle(), sql, -1, &stmt, nullptr);

    if (result != SQLITE_OK) {
        throw DatabaseException("Failed to prepare count statement: " + database_.getLastError());
    }

    result = sqlite3_step(stmt);

    if (result != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        throw DatabaseException("Failed to count pending items: " + database_.getLastError());
    }

    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    return count;
}

TodoItem TodoRepository::readTodoItem(sqlite3_stmt* stmt) {
    int id = sqlite3_column_int(stmt, 0);
    std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

    const unsigned char* desc_ptr = sqlite3_column_text(stmt, 2);
    std::string description = desc_ptr ? reinterpret_cast<const char*>(desc_ptr) : "";

    bool completed = sqlite3_column_int(stmt, 3) != 0;
    std::time_t created_at_unix = sqlite3_column_int64(stmt, 4);

    TodoItem::TimePoint created_at = TodoItem::fromUnixTime(created_at_unix);

    return TodoItem(id, title, description, completed, created_at);
}

} // namespace todolist
