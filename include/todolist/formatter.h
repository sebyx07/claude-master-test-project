#ifndef TODOLIST_FORMATTER_H
#define TODOLIST_FORMATTER_H

#include "todolist/todo_item.h"
#include <string>
#include <vector>

namespace todolist {

/**
 * @brief ANSI color codes for terminal output
 */
namespace Color {
    constexpr const char* RESET = "\033[0m";
    constexpr const char* BOLD = "\033[1m";
    constexpr const char* DIM = "\033[2m";

    // Colors
    constexpr const char* RED = "\033[31m";
    constexpr const char* GREEN = "\033[32m";
    constexpr const char* YELLOW = "\033[33m";
    constexpr const char* BLUE = "\033[34m";
    constexpr const char* MAGENTA = "\033[35m";
    constexpr const char* CYAN = "\033[36m";
    constexpr const char* WHITE = "\033[37m";

    // Bright colors
    constexpr const char* BRIGHT_RED = "\033[91m";
    constexpr const char* BRIGHT_GREEN = "\033[92m";
    constexpr const char* BRIGHT_YELLOW = "\033[93m";
    constexpr const char* BRIGHT_BLUE = "\033[94m";
    constexpr const char* BRIGHT_MAGENTA = "\033[95m";
    constexpr const char* BRIGHT_CYAN = "\033[96m";
}

/**
 * @brief Formatter for CLI output
 *
 * Provides utilities for formatting todo items and messages
 * with ANSI colors for better readability.
 */
class Formatter {
public:
    /**
     * @brief Constructor
     * @param useColor Whether to use ANSI color codes (default: true)
     */
    explicit Formatter(bool useColor = true);

    /**
     * @brief Format a single todo item for display
     * @param item The todo item to format
     * @param showDescription Whether to include the description
     * @return Formatted string representation
     */
    std::string formatTodoItem(const TodoItem& item, bool showDescription = true) const;

    /**
     * @brief Format a list of todo items as a table
     * @param items The todo items to format
     * @param showDescription Whether to include descriptions
     * @return Formatted table string
     */
    std::string formatTodoList(const std::vector<TodoItem>& items, bool showDescription = false) const;

    /**
     * @brief Format a success message
     * @param message The success message
     * @return Formatted message with green color
     */
    std::string formatSuccess(const std::string& message) const;

    /**
     * @brief Format an error message
     * @param message The error message
     * @return Formatted message with red color
     */
    std::string formatError(const std::string& message) const;

    /**
     * @brief Format a warning message
     * @param message The warning message
     * @return Formatted message with yellow color
     */
    std::string formatWarning(const std::string& message) const;

    /**
     * @brief Format an info message
     * @param message The info message
     * @return Formatted message with blue color
     */
    std::string formatInfo(const std::string& message) const;

    /**
     * @brief Format a header/title
     * @param title The header text
     * @return Formatted header with bold styling
     */
    std::string formatHeader(const std::string& title) const;

    /**
     * @brief Colorize text with a specific color
     * @param text The text to colorize
     * @param color The ANSI color code
     * @return Colorized text
     */
    std::string colorize(const std::string& text, const char* color) const;

    /**
     * @brief Create a separator line
     * @param length Length of the separator (default: 80)
     * @param character Character to use (default: '-')
     * @return Separator line string
     */
    std::string separator(size_t length = 80, char character = '-') const;

    /**
     * @brief Enable or disable color output
     * @param enabled Whether to enable colors
     */
    void setColorEnabled(bool enabled);

    /**
     * @brief Check if color output is enabled
     * @return true if colors are enabled
     */
    bool isColorEnabled() const;

private:
    bool useColor_;

    /**
     * @brief Apply color if color output is enabled
     * @param color The color code
     * @return The color code if enabled, empty string otherwise
     */
    const char* applyColor(const char* color) const;
};

} // namespace todolist

#endif // TODOLIST_FORMATTER_H
