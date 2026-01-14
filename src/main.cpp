#include <iostream>
#include <string>
#include <vector>
#include "todolist/version.h"

void printUsage(const std::string& programName) {
    std::cout << "TodoList CLI v" << TODOLIST_VERSION << "\n\n";
    std::cout << "Usage: " << programName << " <command> [arguments]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  add <title> [description]  - Add a new todo item\n";
    std::cout << "  list [all|completed|pending] - List todo items\n";
    std::cout << "  complete <id>              - Mark todo as completed\n";
    std::cout << "  delete <id>                - Delete a todo item\n";
    std::cout << "  search <query>             - Search todos by title\n";
    std::cout << "  help                       - Show this help message\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: No command specified\n\n";
        printUsage(argv[0]);
        return 1;
    }

    std::string command = argv[1];

    if (command == "help" || command == "--help") {
        printUsage(argv[0]);
        return 0;
    }

    if (command == "version" || command == "--version") {
        std::cout << "TodoList CLI v" << TODOLIST_VERSION << "\n";
        return 0;
    }

    // Placeholder for actual command handling (will be implemented in PR 3)
    std::cout << "Command '" << command << "' will be implemented in a future update.\n";
    std::cout << "This is the foundation build (PR 1).\n";

    return 0;
}
