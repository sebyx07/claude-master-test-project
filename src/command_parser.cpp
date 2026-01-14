#include "todolist/command_parser.h"
#include <algorithm>
#include <sstream>

namespace todolist {

bool ParsedCommand::hasFlag(const std::string& flag) const {
    return options.find(flag) != options.end();
}

std::optional<std::string> ParsedCommand::getOption(const std::string& option) const {
    auto it = options.find(option);
    if (it != options.end()) {
        return it->second;
    }
    return std::nullopt;
}

ParsedCommand CommandParser::parse(int argc, char* argv[]) const {
    std::vector<std::string> args;
    // Skip program name (argv[0])
    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    return parse(args);
}

ParsedCommand CommandParser::parse(const std::vector<std::string>& args) const {
    ParsedCommand result;
    result.command = Command::UNKNOWN;

    if (args.empty()) {
        result.command = Command::HELP;
        return result;
    }

    // First argument is the command
    const std::string& cmdStr = args[0];

    // Check for special flags that override command parsing
    if (cmdStr == "-h" || cmdStr == "--help") {
        result.command = Command::HELP;
        return result;
    }

    if (cmdStr == "-v" || cmdStr == "--version") {
        result.command = Command::VERSION;
        return result;
    }

    // Parse the command
    result.command = stringToCommand(cmdStr);

    // Parse remaining arguments
    for (size_t i = 1; i < args.size(); ++i) {
        const std::string& arg = args[i];

        if (isFlag(arg)) {
            std::string flagName = parseFlag(arg);

            // Check if next argument is the value for this flag
            if (i + 1 < args.size() && !isFlag(args[i + 1])) {
                result.options[flagName] = args[i + 1];
                ++i; // Skip the value argument
            } else {
                // Boolean flag (no value)
                result.options[flagName] = "true";
            }
        } else {
            // Positional argument
            result.args.push_back(arg);
        }
    }

    return result;
}

std::string CommandParser::commandToString(Command cmd) {
    switch (cmd) {
        case Command::ADD:      return "add";
        case Command::LIST:     return "list";
        case Command::COMPLETE: return "complete";
        case Command::DELETE:   return "delete";
        case Command::SEARCH:   return "search";
        case Command::HELP:     return "help";
        case Command::VERSION:  return "version";
        case Command::UNKNOWN:  return "unknown";
    }
    return "unknown";
}

Command CommandParser::stringToCommand(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    // Support command aliases
    if (lower == "add" || lower == "a" || lower == "new") {
        return Command::ADD;
    } else if (lower == "list" || lower == "l" || lower == "ls") {
        return Command::LIST;
    } else if (lower == "complete" || lower == "c" || lower == "done") {
        return Command::COMPLETE;
    } else if (lower == "delete" || lower == "d" || lower == "del" || lower == "rm") {
        return Command::DELETE;
    } else if (lower == "search" || lower == "s" || lower == "find") {
        return Command::SEARCH;
    } else if (lower == "help" || lower == "h") {
        return Command::HELP;
    } else if (lower == "version" || lower == "v") {
        return Command::VERSION;
    }

    return Command::UNKNOWN;
}

std::string CommandParser::getCommandHelp(Command cmd) {
    switch (cmd) {
        case Command::ADD:
            return "add <title> [description]\n"
                   "  Add a new todo item.\n"
                   "  Aliases: a, new\n"
                   "  Examples:\n"
                   "    todo add \"Buy groceries\"\n"
                   "    todo add \"Fix bug\" \"Fix the memory leak in parser\"";

        case Command::LIST:
            return "list [filter]\n"
                   "  List todo items. Optional filter: all, completed, pending.\n"
                   "  Aliases: l, ls\n"
                   "  Examples:\n"
                   "    todo list\n"
                   "    todo list completed\n"
                   "    todo list pending";

        case Command::COMPLETE:
            return "complete <id>\n"
                   "  Mark a todo item as completed.\n"
                   "  Aliases: c, done\n"
                   "  Examples:\n"
                   "    todo complete 1\n"
                   "    todo done 42";

        case Command::DELETE:
            return "delete <id>\n"
                   "  Delete a todo item.\n"
                   "  Aliases: d, del, rm\n"
                   "  Examples:\n"
                   "    todo delete 1\n"
                   "    todo rm 42";

        case Command::SEARCH:
            return "search <query>\n"
                   "  Search for todo items by title.\n"
                   "  Aliases: s, find\n"
                   "  Examples:\n"
                   "    todo search \"groceries\"\n"
                   "    todo find bug";

        case Command::HELP:
            return "help [command]\n"
                   "  Display help information.\n"
                   "  Examples:\n"
                   "    todo help\n"
                   "    todo help add";

        case Command::VERSION:
            return "version\n"
                   "  Display version information.\n"
                   "  Example:\n"
                   "    todo version";

        case Command::UNKNOWN:
            return "Unknown command. Use 'todo help' for usage information.";
    }
    return "";
}

std::string CommandParser::getUsage() {
    std::ostringstream oss;
    oss << "Todo List - A simple command-line todo list manager\n\n";
    oss << "Usage: todo <command> [arguments] [options]\n\n";
    oss << "Commands:\n\n";

    oss << getCommandHelp(Command::ADD) << "\n\n";
    oss << getCommandHelp(Command::LIST) << "\n\n";
    oss << getCommandHelp(Command::COMPLETE) << "\n\n";
    oss << getCommandHelp(Command::DELETE) << "\n\n";
    oss << getCommandHelp(Command::SEARCH) << "\n\n";
    oss << getCommandHelp(Command::HELP) << "\n\n";
    oss << getCommandHelp(Command::VERSION) << "\n";

    return oss.str();
}

bool CommandParser::isFlag(const std::string& str) {
    return !str.empty() && str[0] == '-';
}

std::string CommandParser::parseFlag(const std::string& flag) {
    std::string result = flag;

    // Remove leading dashes
    while (!result.empty() && result[0] == '-') {
        result = result.substr(1);
    }

    return result;
}

} // namespace todolist
