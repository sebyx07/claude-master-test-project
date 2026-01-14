#ifndef TODOLIST_CLI_HANDLER_H
#define TODOLIST_CLI_HANDLER_H

#include "todolist/command_parser.h"
#include "todolist/todo_repository.h"
#include "todolist/formatter.h"
#include <memory>
#include <string>

namespace todolist {

/**
 * @brief Handler for CLI commands
 *
 * Processes parsed commands and executes the corresponding operations
 * on the todo repository.
 */
class CliHandler {
public:
    /**
     * @brief Constructor
     * @param repository The todo repository for data access
     * @param formatter The formatter for output (optional)
     */
    explicit CliHandler(TodoRepository& repository,
                       std::unique_ptr<Formatter> formatter = nullptr);

    /**
     * @brief Execute a parsed command
     * @param cmd The parsed command to execute
     * @return Exit code (0 for success, non-zero for error)
     */
    int execute(const ParsedCommand& cmd);

    /**
     * @brief Handle the add command
     * @param args Command arguments (title, description)
     * @return Success message
     */
    std::string handleAdd(const std::vector<std::string>& args);

    /**
     * @brief Handle the list command
     * @param args Command arguments (optional filter)
     * @return Formatted list of todos
     */
    std::string handleList(const std::vector<std::string>& args);

    /**
     * @brief Handle the complete command
     * @param args Command arguments (todo ID)
     * @return Success message
     */
    std::string handleComplete(const std::vector<std::string>& args);

    /**
     * @brief Handle the delete command
     * @param args Command arguments (todo ID)
     * @return Success message
     */
    std::string handleDelete(const std::vector<std::string>& args);

    /**
     * @brief Handle the search command
     * @param args Command arguments (search query)
     * @return Formatted search results
     */
    std::string handleSearch(const std::vector<std::string>& args);

    /**
     * @brief Handle the help command
     * @param args Command arguments (optional command name)
     * @return Help text
     */
    std::string handleHelp(const std::vector<std::string>& args);

    /**
     * @brief Handle the version command
     * @return Version information
     */
    std::string handleVersion();

    /**
     * @brief Get the formatter
     * @return Reference to the formatter
     */
    Formatter& getFormatter();

private:
    TodoRepository& repository_;
    std::unique_ptr<Formatter> formatter_;

    /**
     * @brief Parse an ID argument
     * @param idStr The ID string
     * @return The parsed ID
     * @throws ValidationException if the ID is invalid
     */
    int parseId(const std::string& idStr) const;

    /**
     * @brief Validate that arguments list is not empty
     * @param args The arguments to validate
     * @param message Error message if empty
     * @throws ValidationException if args is empty
     */
    void requireArgs(const std::vector<std::string>& args, const std::string& message) const;
};

} // namespace todolist

#endif // TODOLIST_CLI_HANDLER_H
