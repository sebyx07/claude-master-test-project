#ifndef TODOLIST_TODO_REPOSITORY_H
#define TODOLIST_TODO_REPOSITORY_H

#include "todolist/todo_item.h"
#include "todolist/database.h"
#include <vector>
#include <optional>
#include <memory>

namespace todolist {

/**
 * @brief Repository for CRUD operations on TodoItem objects
 *
 * This class implements the repository pattern, providing a clean
 * abstraction over the database layer for managing todo items.
 */
class TodoRepository {
public:
    /**
     * @brief Constructor
     * @param database Reference to the database connection
     */
    explicit TodoRepository(Database& database);

    /**
     * @brief Create a new todo item in the database
     * @param item TodoItem to create (id will be set by database)
     * @return The created item with its database-assigned id
     * @throws DatabaseException if creation fails
     */
    TodoItem create(const TodoItem& item);

    /**
     * @brief Find a todo item by its id
     * @param id The todo item id
     * @return Optional containing the item if found, empty otherwise
     * @throws DatabaseException if query fails
     */
    std::optional<TodoItem> findById(int id);

    /**
     * @brief Retrieve all todo items
     * @return Vector of all todo items
     * @throws DatabaseException if query fails
     */
    std::vector<TodoItem> findAll();

    /**
     * @brief Find all completed todo items
     * @return Vector of completed items
     * @throws DatabaseException if query fails
     */
    std::vector<TodoItem> findCompleted();

    /**
     * @brief Find all pending (not completed) todo items
     * @return Vector of pending items
     * @throws DatabaseException if query fails
     */
    std::vector<TodoItem> findPending();

    /**
     * @brief Search todo items by title
     * @param query Search query (case-insensitive, partial match)
     * @return Vector of matching items
     * @throws DatabaseException if query fails
     */
    std::vector<TodoItem> findByTitle(const std::string& query);

    /**
     * @brief Update an existing todo item
     * @param item The item to update (must have valid id)
     * @return true if updated, false if item not found
     * @throws DatabaseException if update fails
     */
    bool update(const TodoItem& item);

    /**
     * @brief Delete a todo item by id
     * @param id The id of the item to delete
     * @return true if deleted, false if item not found
     * @throws DatabaseException if deletion fails
     */
    bool remove(int id);

    /**
     * @brief Count total number of todo items
     * @return Number of items
     * @throws DatabaseException if query fails
     */
    int count();

    /**
     * @brief Count completed todo items
     * @return Number of completed items
     * @throws DatabaseException if query fails
     */
    int countCompleted();

    /**
     * @brief Count pending todo items
     * @return Number of pending items
     * @throws DatabaseException if query fails
     */
    int countPending();

private:
    /**
     * @brief Helper to read a TodoItem from a prepared statement
     * @param stmt SQLite prepared statement
     * @return TodoItem constructed from the current row
     */
    TodoItem readTodoItem(sqlite3_stmt* stmt);

    Database& database_;
};

} // namespace todolist

#endif // TODOLIST_TODO_REPOSITORY_H
