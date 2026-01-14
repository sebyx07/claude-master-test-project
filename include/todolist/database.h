/**
 * @file database.h
 * @brief SQLite database connection and management
 *
 * Provides RAII-based database connection management with automatic
 * schema initialization and error handling.
 */

#ifndef TODOLIST_DATABASE_H
#define TODOLIST_DATABASE_H

#include <string>
#include <memory>
#include <stdexcept>

// Forward declaration to avoid exposing SQLite3 in the header
struct sqlite3;
struct sqlite3_stmt;

namespace todolist {

/**
 * @brief Exception thrown when database operations fail
 */
class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * @brief RAII wrapper for SQLite database connection
 *
 * This class manages the lifecycle of a SQLite database connection,
 * ensuring proper cleanup through RAII principles. It also handles
 * database initialization and schema migration.
 */
class Database {
public:
    /**
     * @brief Open or create a database at the specified path
     * @param db_path Path to the database file
     * @throws DatabaseException if connection fails
     */
    explicit Database(const std::string& db_path);

    /**
     * @brief Destructor - closes the database connection
     */
    ~Database();

    // Disable copy (database connections shouldn't be copied)
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // Enable move
    Database(Database&& other) noexcept;
    Database& operator=(Database&& other) noexcept;

    /**
     * @brief Get the raw SQLite3 connection handle
     * @return sqlite3 pointer
     *
     * This is used by the repository layer to execute queries.
     * The caller does not own this pointer.
     */
    sqlite3* getHandle() const { return db_; }

    /**
     * @brief Check if the database connection is open
     * @return true if connected, false otherwise
     */
    bool isOpen() const { return db_ != nullptr; }

    /**
     * @brief Execute a simple SQL statement without parameters
     * @param sql SQL statement to execute
     * @throws DatabaseException if execution fails
     */
    void execute(const std::string& sql);

    /**
     * @brief Get the last error message from SQLite
     * @return Error message string
     */
    std::string getLastError() const;

private:
    /**
     * @brief Initialize database schema if needed
     *
     * Creates the todos table if it doesn't exist and runs any
     * necessary migrations.
     */
    void initializeSchema();

    sqlite3* db_;
};

} // namespace todolist

#endif // TODOLIST_DATABASE_H
