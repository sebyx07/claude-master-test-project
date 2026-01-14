#ifndef TODOLIST_COMMAND_PARSER_H
#define TODOLIST_COMMAND_PARSER_H

#include <string>
#include <vector>
#include <optional>
#include <map>

namespace todolist {

/**
 * @brief Enumeration of supported CLI commands
 */
enum class Command {
    ADD,        ///< Add a new todo item
    LIST,       ///< List todo items
    COMPLETE,   ///< Mark a todo as completed
    DELETE,     ///< Delete a todo item
    SEARCH,     ///< Search for todo items
    HELP,       ///< Display help information
    VERSION,    ///< Display version information
    UNKNOWN     ///< Unknown or invalid command
};

/**
 * @brief Result of parsing command-line arguments
 */
struct ParsedCommand {
    Command command;                              ///< The parsed command
    std::vector<std::string> args;                ///< Positional arguments
    std::map<std::string, std::string> options;   ///< Named options (flags)

    /**
     * @brief Check if a flag/option is present
     * @param flag The flag name (without dashes)
     * @return true if the flag was provided
     */
    bool hasFlag(const std::string& flag) const;

    /**
     * @brief Get the value of a named option
     * @param option The option name
     * @return The option value if present, std::nullopt otherwise
     */
    std::optional<std::string> getOption(const std::string& option) const;
};

/**
 * @brief Parser for command-line arguments
 *
 * Parses command-line arguments into structured commands with arguments and options.
 * Supports both positional arguments and named flags.
 *
 * Example usage:
 * @code
 *   CommandParser parser;
 *   auto result = parser.parse(argc, argv);
 *   if (result.command == Command::ADD) {
 *       // Handle add command
 *   }
 * @endcode
 */
class CommandParser {
public:
    /**
     * @brief Default constructor
     */
    CommandParser() = default;

    /**
     * @brief Parse command-line arguments
     * @param argc Argument count
     * @param argv Argument values
     * @return Parsed command structure
     */
    ParsedCommand parse(int argc, char* argv[]) const;

    /**
     * @brief Parse command-line arguments from a vector
     * @param args Vector of argument strings
     * @return Parsed command structure
     */
    ParsedCommand parse(const std::vector<std::string>& args) const;

    /**
     * @brief Convert a command enum to its string representation
     * @param cmd The command to convert
     * @return String representation of the command
     */
    static std::string commandToString(Command cmd);

    /**
     * @brief Convert a string to a command enum
     * @param str The string to convert
     * @return The corresponding command, or Command::UNKNOWN if not recognized
     */
    static Command stringToCommand(const std::string& str);

    /**
     * @brief Get help text for a specific command
     * @param cmd The command to get help for
     * @return Help text describing the command usage
     */
    static std::string getCommandHelp(Command cmd);

    /**
     * @brief Get full usage information for all commands
     * @return Complete usage/help text
     */
    static std::string getUsage();

private:
    /**
     * @brief Check if a string is a flag (starts with -)
     * @param str The string to check
     * @return true if it's a flag
     */
    static bool isFlag(const std::string& str);

    /**
     * @brief Parse a flag string (remove leading dashes)
     * @param flag The flag string
     * @return The flag name without dashes
     */
    static std::string parseFlag(const std::string& flag);
};

} // namespace todolist

#endif // TODOLIST_COMMAND_PARSER_H
