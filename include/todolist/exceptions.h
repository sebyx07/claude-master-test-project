#ifndef TODOLIST_EXCEPTIONS_H
#define TODOLIST_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace todolist {

/**
 * @brief Base exception class for todolist errors
 */
class TodoListException : public std::runtime_error {
public:
    explicit TodoListException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * @brief Exception thrown for validation errors
 */
class ValidationException : public TodoListException {
public:
    explicit ValidationException(const std::string& message)
        : TodoListException(message) {}
};

/**
 * @brief Exception thrown when a todo item is not found
 */
class NotFoundException : public TodoListException {
public:
    explicit NotFoundException(const std::string& message)
        : TodoListException(message) {}

    explicit NotFoundException(int id)
        : TodoListException("Todo item with ID " + std::to_string(id) + " not found") {}
};

/**
 * @brief Exception thrown for invalid command usage
 */
class InvalidCommandException : public TodoListException {
public:
    explicit InvalidCommandException(const std::string& message)
        : TodoListException(message) {}
};

} // namespace todolist

#endif // TODOLIST_EXCEPTIONS_H
