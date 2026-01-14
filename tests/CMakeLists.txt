# GoogleTest setup
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Enable GoogleTest
include(GoogleTest)

# Test executable with all test files
add_executable(todolist_tests
    test_main.cpp
    test_todo_item.cpp
    test_database.cpp
    test_todo_repository.cpp
    test_command_parser.cpp
    test_cli_handler.cpp
)

# Add core library sources to test executable
target_sources(todolist_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/src/todo_item.cpp
    ${CMAKE_SOURCE_DIR}/src/database.cpp
    ${CMAKE_SOURCE_DIR}/src/todo_repository.cpp
    ${CMAKE_SOURCE_DIR}/src/command_parser.cpp
    ${CMAKE_SOURCE_DIR}/src/formatter.cpp
    ${CMAKE_SOURCE_DIR}/src/cli_handler.cpp
)

target_include_directories(todolist_tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${SQLite3_INCLUDE_DIRS}
)

target_link_libraries(todolist_tests
    PRIVATE
        gtest
        gtest_main
        SQLite::SQLite3
)

# Discover tests
gtest_discover_tests(todolist_tests)
