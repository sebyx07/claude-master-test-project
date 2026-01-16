#include "todolist/cli_handler.h"
#include "todolist/exceptions.h"
#include "todolist/version.h"
#include <iostream>
#include <sstream>

namespace todolist {

CliHandler::CliHandler(TodoRepository& repository,
                       std::unique_ptr<Formatter> formatter)
    : repository_(repository)
    , formatter_(formatter ? std::move(formatter) : std::make_unique<Formatter>()) {
}

int CliHandler::execute(const ParsedCommand& cmd) {
    try {
        std::string output;

        // Check for --help flag to show context-sensitive help
        if (cmd.hasFlag("help") || cmd.hasFlag("h")) {
            if (cmd.command != Command::HELP && cmd.command != Command::UNKNOWN) {
                std::string cmdStr = CommandParser::commandToString(cmd.command);
                std::ostringstream oss;
                oss << formatter_->formatHeader("Help for: " + cmdStr) << "\n";
                oss << formatter_->separator() << "\n\n";
                oss << CommandParser::getCommandHelp(cmd.command);
                std::cout << oss.str() << std::endl;
                return 0;
            }
        }

        switch (cmd.command) {
            case Command::ADD:
                output = handleAdd(cmd.args);
                break;

            case Command::LIST:
                output = handleList(cmd.args);
                break;

            case Command::COMPLETE:
                output = handleComplete(cmd.args);
                break;

            case Command::DELETE:
                output = handleDelete(cmd.args);
                break;

            case Command::SEARCH:
                output = handleSearch(cmd.args);
                break;

            case Command::HELP:
                output = handleHelp(cmd.args);
                break;

            case Command::VERSION:
                output = handleVersion();
                break;

            case Command::UNKNOWN:
                output = formatter_->formatError("Unknown command. Use 'help' for usage information.");
                std::cout << output << std::endl;
                return 1;
        }

        std::cout << output << std::endl;
        return 0;

    } catch (const ValidationException& e) {
        std::cout << formatter_->formatError(e.what()) << std::endl;
        return 1;
    } catch (const NotFoundException& e) {
        std::cout << formatter_->formatError(e.what()) << std::endl;
        return 1;
    } catch (const DatabaseException& e) {
        std::cout << formatter_->formatError("Database error: " + std::string(e.what())) << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cout << formatter_->formatError("Unexpected error: " + std::string(e.what())) << std::endl;
        return 1;
    }
}

std::string CliHandler::handleAdd(const std::vector<std::string>& args) {
    requireArgs(args, "Title is required. Usage: add <title> [description]");

    const std::string& title = args[0];
    std::string description;

    if (args.size() > 1) {
        description = args[1];
    }

    // Validate title
    if (title.empty()) {
        throw ValidationException("Title cannot be empty");
    }

    // Create the todo item
    TodoItem item(title, description);
    repository_.create(item);

    std::ostringstream oss;
    oss << formatter_->formatSuccess("Todo item created successfully");
    oss << "\n\n";
    oss << formatter_->formatTodoItem(item, true);

    return oss.str();
}

std::string CliHandler::handleList(const std::vector<std::string>& args) {
    std::string filter = "all";
    if (!args.empty()) {
        filter = args[0];
    }

    std::vector<TodoItem> items;

    if (filter == "all") {
        items = repository_.findAll();
    } else if (filter == "completed") {
        items = repository_.findCompleted();
    } else if (filter == "pending") {
        items = repository_.findPending();
    } else {
        throw ValidationException("Invalid filter. Use: all, completed, or pending");
    }

    return formatter_->formatTodoList(items, false);
}

std::string CliHandler::handleComplete(const std::vector<std::string>& args) {
    requireArgs(args, "Todo ID is required. Usage: complete <id>");

    int id = parseId(args[0]);

    // Find the item
    auto item = repository_.findById(id);
    if (!item) {
        throw NotFoundException(id);
    }

    // Check if already completed
    if (item->isCompleted()) {
        throw ValidationException("Todo item is already completed");
    }

    // Mark as completed
    item->setCompleted(true);
    repository_.update(*item);

    std::ostringstream oss;
    oss << formatter_->formatSuccess("Todo item marked as completed");
    oss << "\n\n";
    oss << formatter_->formatTodoItem(*item, true);

    return oss.str();
}

std::string CliHandler::handleDelete(const std::vector<std::string>& args) {
    requireArgs(args, "Todo ID is required. Usage: delete <id>");

    int id = parseId(args[0]);

    // Find the item first to verify it exists
    auto item = repository_.findById(id);
    if (!item) {
        throw NotFoundException(id);
    }

    // Delete the item
    repository_.remove(id);

    std::ostringstream oss;
    oss << formatter_->formatSuccess("Todo item deleted successfully");
    oss << "\n\n";
    oss << formatter_->formatTodoItem(*item, true);

    return oss.str();
}

std::string CliHandler::handleSearch(const std::vector<std::string>& args) {
    requireArgs(args, "Search query is required. Usage: search <query>");

    const std::string& query = args[0];

    if (query.empty()) {
        throw ValidationException("Search query cannot be empty");
    }

    auto items = repository_.findByTitle(query);

    if (items.empty()) {
        return formatter_->formatInfo("No todo items found matching: " + query);
    }

    std::ostringstream oss;
    oss << formatter_->formatHeader("Search Results for: " + query) << "\n";
    oss << formatter_->separator() << "\n\n";
    oss << formatter_->formatTodoList(items, false);

    return oss.str();
}

std::string CliHandler::handleHelp(const std::vector<std::string>& args) {
    if (args.empty()) {
        return CommandParser::getUsage();
    }

    // Get help for specific command
    const std::string& cmdStr = args[0];
    Command cmd = CommandParser::stringToCommand(cmdStr);

    if (cmd == Command::UNKNOWN) {
        throw ValidationException("Unknown command: " + cmdStr);
    }

    std::ostringstream oss;
    oss << formatter_->formatHeader("Help for: " + cmdStr) << "\n";
    oss << formatter_->separator() << "\n\n";
    oss << CommandParser::getCommandHelp(cmd);

    return oss.str();
}

std::string CliHandler::handleVersion() {
    std::ostringstream oss;
    oss << formatter_->formatHeader("Todo List CLI") << "\n";
    oss << "Version: " << TODOLIST_VERSION_MAJOR << "." << TODOLIST_VERSION_MINOR << "." << TODOLIST_VERSION_PATCH << "\n";
    oss << "Build: " << TODOLIST_VERSION;
    return oss.str();
}

Formatter& CliHandler::getFormatter() {
    return *formatter_;
}

int CliHandler::parseId(const std::string& idStr) const {
    try {
        size_t pos;
        int id = std::stoi(idStr, &pos);

        // Check if entire string was consumed
        if (pos != idStr.length()) {
            throw ValidationException("Invalid ID format: " + idStr);
        }

        if (id <= 0) {
            throw ValidationException("ID must be a positive number");
        }

        return id;
    } catch (const std::invalid_argument&) {
        throw ValidationException("Invalid ID format: " + idStr);
    } catch (const std::out_of_range&) {
        throw ValidationException("ID is out of range: " + idStr);
    }
}

void CliHandler::requireArgs(const std::vector<std::string>& args, const std::string& message) const {
    if (args.empty()) {
        throw ValidationException(message);
    }
}

} // namespace todolist
