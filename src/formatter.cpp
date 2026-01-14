#include "todolist/formatter.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace todolist {

Formatter::Formatter(bool useColor)
    : useColor_(useColor) {
}

std::string Formatter::formatTodoItem(const TodoItem& item, bool showDescription) const {
    std::ostringstream oss;

    // ID and status indicator
    oss << applyColor(Color::DIM) << "[" << item.getId() << "]" << applyColor(Color::RESET);
    oss << " ";

    // Checkbox with color
    if (item.isCompleted()) {
        oss << applyColor(Color::BRIGHT_GREEN) << "[✓]" << applyColor(Color::RESET);
    } else {
        oss << applyColor(Color::YELLOW) << "[ ]" << applyColor(Color::RESET);
    }
    oss << " ";

    // Title with strikethrough effect for completed items
    if (item.isCompleted()) {
        oss << applyColor(Color::DIM) << item.getTitle() << applyColor(Color::RESET);
    } else {
        oss << applyColor(Color::BOLD) << item.getTitle() << applyColor(Color::RESET);
    }

    // Description if requested and available
    if (showDescription && !item.getDescription().empty()) {
        oss << "\n    " << applyColor(Color::DIM) << item.getDescription() << applyColor(Color::RESET);
    }

    // Created timestamp
    oss << "\n    " << applyColor(Color::DIM) << "Created: " << item.getFormattedCreatedAt() << applyColor(Color::RESET);

    return oss.str();
}

std::string Formatter::formatTodoList(const std::vector<TodoItem>& items, bool showDescription) const {
    if (items.empty()) {
        return formatInfo("No todo items found.");
    }

    std::ostringstream oss;

    // Header
    oss << formatHeader("Todo Items") << "\n";
    oss << separator() << "\n\n";

    // Count statistics
    size_t completed = std::count_if(items.begin(), items.end(),
                                      [](const TodoItem& item) { return item.isCompleted(); });
    size_t pending = items.size() - completed;

    oss << formatInfo("Total: " + std::to_string(items.size()) + " items");
    oss << " | ";
    oss << colorize(std::to_string(pending) + " pending", Color::YELLOW);
    oss << " | ";
    oss << colorize(std::to_string(completed) + " completed", Color::BRIGHT_GREEN);
    oss << "\n\n";

    // Items
    for (const auto& item : items) {
        oss << formatTodoItem(item, showDescription);
        oss << "\n\n";
    }

    oss << separator();

    return oss.str();
}

std::string Formatter::formatSuccess(const std::string& message) const {
    std::ostringstream oss;
    oss << applyColor(Color::BRIGHT_GREEN) << "✓ " << message << applyColor(Color::RESET);
    return oss.str();
}

std::string Formatter::formatError(const std::string& message) const {
    std::ostringstream oss;
    oss << applyColor(Color::BRIGHT_RED) << "✗ Error: " << message << applyColor(Color::RESET);
    return oss.str();
}

std::string Formatter::formatWarning(const std::string& message) const {
    std::ostringstream oss;
    oss << applyColor(Color::BRIGHT_YELLOW) << "⚠ Warning: " << message << applyColor(Color::RESET);
    return oss.str();
}

std::string Formatter::formatInfo(const std::string& message) const {
    std::ostringstream oss;
    oss << applyColor(Color::BRIGHT_BLUE) << "ℹ " << message << applyColor(Color::RESET);
    return oss.str();
}

std::string Formatter::formatHeader(const std::string& title) const {
    std::ostringstream oss;
    oss << applyColor(Color::BOLD) << applyColor(Color::BRIGHT_CYAN) << title << applyColor(Color::RESET);
    return oss.str();
}

std::string Formatter::colorize(const std::string& text, const char* color) const {
    if (!useColor_) {
        return text;
    }
    std::ostringstream oss;
    oss << color << text << Color::RESET;
    return oss.str();
}

std::string Formatter::separator(size_t length, char character) const {
    return std::string(length, character);
}

void Formatter::setColorEnabled(bool enabled) {
    useColor_ = enabled;
}

bool Formatter::isColorEnabled() const {
    return useColor_;
}

const char* Formatter::applyColor(const char* color) const {
    return useColor_ ? color : "";
}

} // namespace todolist
