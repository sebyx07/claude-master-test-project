# Todo List CLI

[![Built with claude-task-master](https://img.shields.io/badge/Built%20with-claude--task--master-blue)](https://github.com/sebyx07/claude-task-master)
[![Built with Claude Code](https://img.shields.io/badge/Built%20with-Claude%20Code-orange)](https://claude.ai/code)

A production-quality command-line todo list manager built with modern C++17. Lightweight, fast, and easy to use.

> **Note**: This entire project was autonomously built by [claude-task-master](https://github.com/sebyx07/claude-task-master) + [Claude Code](https://claude.ai/code) — from initial planning to final implementation, including all 97 tests, CI/CD setup, and documentation.

## Features

- **Simple Commands**: Add, list, complete, delete, and search todos with intuitive commands
- **Persistent Storage**: SQLite3 database for reliable data persistence
- **Colored Output**: Beautiful ANSI-colored terminal output for better readability
- **Command Aliases**: Short aliases for faster workflow (e.g., `a` for `add`, `l` for `list`)
- **Fast and Lightweight**: Minimal dependencies, compiles quickly, runs instantly
- **Cross-Platform**: Works on Linux, macOS, and Windows
- **Well-Tested**: Comprehensive test suite with 97 unit and integration tests

## Requirements

### Build Requirements

- **CMake**: Version 3.14 or higher
- **C++ Compiler**: GCC 9+, Clang 10+, or MSVC 2019+ with C++17 support
- **SQLite3**: Development libraries (libsqlite3-dev)
- **Ninja**: Build system (optional but recommended)

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install cmake ninja-build libsqlite3-dev g++
```

**macOS (Homebrew):**
```bash
brew install cmake ninja sqlite3
```

**Fedora/RHEL:**
```bash
sudo dnf install cmake ninja-build sqlite-devel gcc-c++
```

**Windows (vcpkg):**
```bash
vcpkg install sqlite3:x64-windows
```

## Building from Source

### Quick Start

```bash
# Clone the repository
git clone https://github.com/sebyx07/claude-master-test-project.git
cd claude-master-test-project

# Build with CMake and Ninja
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Install (optional)
sudo cmake --install build --prefix /usr/local

# Run
./build/bin/todolist --help
```

### Build Options

**Debug Build:**
```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

**Without Ninja:**
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Running Tests

The project includes 97 comprehensive tests using GoogleTest:

```bash
# Build and run tests
cmake --build build
ctest --test-dir build --output-on-failure

# Or run the test binary directly
./build/tests/todolist_tests
```

## Usage

### Basic Commands

**Add a new todo:**
```bash
todolist add "Buy groceries"
todolist add "Fix memory leak" "Check the parser module"
```

**List all todos:**
```bash
todolist list
```

**List completed/pending todos:**
```bash
todolist list completed
todolist list pending
```

**Mark a todo as completed:**
```bash
todolist complete 1
```

**Delete a todo:**
```bash
todolist delete 1
```

**Search todos:**
```bash
todolist search "groceries"
```

**Get help:**
```bash
todolist help
todolist help add
```

**Display version:**
```bash
todolist --version
```

### Command Aliases

Save keystrokes with short aliases:

- `add` → `a`, `new`
- `list` → `l`, `ls`
- `complete` → `c`, `done`
- `delete` → `d`, `del`, `rm`
- `search` → `s`, `find`

Examples:
```bash
todolist a "New task"
todolist l
todolist c 1
todolist rm 2
```

### Example Session

```bash
$ todolist add "Buy groceries" "Milk, bread, eggs"
✓ Todo item created successfully

$ todolist add "Fix memory leak" "Check the parser module"
✓ Todo item created successfully

$ todolist add "Write documentation"
✓ Todo item created successfully

$ todolist list
Todo Items
--------------------------------------------------------------------------------

ℹ Total: 3 items | 3 pending | 0 completed

[1] [ ] Buy groceries
    Created: 2026-01-14 17:19:38

[2] [ ] Fix memory leak
    Created: 2026-01-14 17:19:38

[3] [ ] Write documentation
    Created: 2026-01-14 17:19:38

--------------------------------------------------------------------------------

$ todolist complete 1
✓ Todo item marked as completed

$ todolist list
Todo Items
--------------------------------------------------------------------------------

ℹ Total: 3 items | 2 pending | 1 completed

[1] [✓] Buy groceries
    Created: 2026-01-14 17:19:38

[2] [ ] Fix memory leak
    Created: 2026-01-14 17:19:38

[3] [ ] Write documentation
    Created: 2026-01-14 17:19:38

--------------------------------------------------------------------------------
```

## Configuration

### Database Location

By default, the database is stored at `~/.todolist.db`. You can customize this location using the `TODOLIST_DB` environment variable:

```bash
export TODOLIST_DB=/path/to/your/todos.db
todolist add "Task in custom database"
```

## Architecture

The project follows modern C++17 best practices:

- **Repository Pattern**: Clean separation between data access and business logic
- **RAII**: Automatic resource management for database connections
- **Smart Pointers**: No raw pointers for ownership (`std::unique_ptr`, `std::shared_ptr`)
- **Custom Exceptions**: Type-safe error handling hierarchy
- **Prepared Statements**: SQL injection prevention
- **Comprehensive Testing**: 97 unit and integration tests

### Project Structure

```
.
├── src/                    # Source files
│   ├── main.cpp           # Application entry point
│   ├── todo_item.cpp      # Todo data model
│   ├── database.cpp       # SQLite database layer
│   ├── todo_repository.cpp # Data access layer
│   ├── command_parser.cpp # Command-line parsing
│   ├── cli_handler.cpp    # Command handlers
│   └── formatter.cpp      # Output formatting
├── include/todolist/       # Header files
│   ├── version.h
│   ├── todo_item.h
│   ├── database.h
│   ├── todo_repository.h
│   ├── command_parser.h
│   ├── cli_handler.h
│   ├── formatter.h
│   └── exceptions.h
├── tests/                  # Test files (GoogleTest)
├── cmake/                  # CMake modules
└── .github/workflows/      # CI/CD configuration
```

## Troubleshooting

### Build Issues

**SQLite3 not found:**
```bash
# Ubuntu/Debian
sudo apt-get install libsqlite3-dev

# macOS
brew install sqlite3

# Set manually if needed
cmake -B build -DSQLite3_INCLUDE_DIR=/path/to/include -DSQLite3_LIBRARY=/path/to/libsqlite3.so
```

**Compiler version too old:**
```bash
# Check your compiler version
g++ --version
clang++ --version

# GCC 9+ or Clang 10+ required for C++17 support
```

**Ninja not found:**
```bash
# Build without Ninja
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Runtime Issues

**Database permission error:**
```bash
# Check directory permissions
ls -la ~/.todolist.db

# Or use a custom location
export TODOLIST_DB=/tmp/todolist.db
```

**No color output:**
- Colors are automatically disabled when output is piped or redirected
- Check that your terminal supports ANSI color codes

## Development

### Code Style

- C++17 standard
- Clang-format for consistent formatting
- Compiler warnings enabled (`-Wall -Wextra -Wpedantic`)
- No warnings in production code

### Contributing

This is a demonstration project, but contributions are welcome:

1. Fork the repository
2. Create a feature branch
3. Make your changes with tests
4. Ensure CI passes
5. Submit a pull request

### Running Static Analysis

```bash
# cppcheck
cppcheck --enable=all --std=c++17 src/ include/

# clang-tidy
clang-tidy src/*.cpp -- -Iinclude -std=c++17
```

### Memory Leak Detection

```bash
# Build with debug symbols
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build

# Run with valgrind
valgrind --leak-check=full ./build/bin/todolist list
```

## Continuous Integration

The project uses GitHub Actions for CI with the following checks:

- ✅ Build with GCC and Clang on Ubuntu
- ✅ Run full test suite (97 tests)
- ✅ Zero compiler warnings with strict flags
- ✅ CMake configuration validation

## License

MIT License - see LICENSE file for details

## Acknowledgments

- 🤖 **Autonomously built with [claude-task-master](https://github.com/sebyx07/claude-task-master)** - AI-powered autonomous development
- 🧠 **Powered by [Claude Code](https://claude.ai/code)** - Anthropic's AI coding assistant
- Built with [SQLite](https://www.sqlite.org/) - Public domain
- Tested with [GoogleTest](https://github.com/google/googletest) - BSD-3-Clause
- Built with [CMake](https://cmake.org/) - BSD-3-Clause

## Version History

- **1.0.0** (2026-01-14) - Initial release
  - Core CRUD operations
  - SQLite persistence
  - Colored terminal output
  - Command aliases
  - Comprehensive test suite
