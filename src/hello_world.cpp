#include "todolist/hello_world.h"
#include <algorithm>
#include <cctype>

namespace todolist {

std::string printHelloWorld() {
    return "Hello from claude-task-master test!";
}

std::string hello(const std::string& name) {
    // Trim whitespace from the input
    std::string trimmed = name;

    // Trim leading whitespace
    trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));

    // Trim trailing whitespace
    trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), trimmed.end());

    // Return generic greeting if name is empty after trimming
    if (trimmed.empty()) {
        return "Hello, there!";
    }

    return "Hello, " + trimmed + "!";
}

} // namespace todolist
