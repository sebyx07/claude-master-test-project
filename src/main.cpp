#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>
#include <unistd.h>
#include "todolist/command_parser.h"
#include "todolist/cli_handler.h"
#include "todolist/database.h"
#include "todolist/todo_repository.h"
#include "todolist/formatter.h"

namespace {

/**
 * @brief Get the database file path
 *
 * Checks the TODOLIST_DB environment variable first, otherwise uses
 * a default location in the user's home directory.
 */
std::string getDatabasePath() {
    const char* envPath = std::getenv("TODOLIST_DB");
    if (envPath != nullptr) {
        return envPath;
    }

    // Default to current directory for now
    // In a production app, we'd use $HOME/.local/share/todolist/todos.db or similar
    return "todos.db";
}

} // anonymous namespace

int main(int argc, char* argv[]) {
    try {
        // Parse command-line arguments
        todolist::CommandParser parser;
        auto parsedCmd = parser.parse(argc, argv);

        // Set up database and repository
        std::string dbPath = getDatabasePath();
        todolist::Database database(dbPath);
        todolist::TodoRepository repository(database);

        // Set up formatter (detect if output is a TTY for color support)
        bool useColor = isatty(fileno(stdout));
        auto formatter = std::make_unique<todolist::Formatter>(useColor);

        // Set up CLI handler
        todolist::CliHandler handler(repository, std::move(formatter));

        // Execute the command
        return handler.execute(parsedCmd);

    } catch (const todolist::DatabaseException& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
        std::cerr << "Please check that the database file is accessible and not corrupted." << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}
