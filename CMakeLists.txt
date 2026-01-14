cmake_minimum_required(VERSION 3.14)
project(TodoList VERSION 1.0.0 LANGUAGES CXX)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Prefer Ninja generator but work with others
if(NOT CMAKE_GENERATOR MATCHES "Ninja")
    message(STATUS "Consider using Ninja generator for faster builds: cmake -G Ninja")
endif()

# Add CMake module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find SQLite3
find_package(SQLite3 REQUIRED)

# Add subdirectories
add_subdirectory(src)

# Enable testing
enable_testing()
add_subdirectory(tests)

# Installation rules
install(TARGETS todolist
    RUNTIME DESTINATION bin
)
