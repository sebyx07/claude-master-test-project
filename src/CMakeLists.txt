# Source files for the todolist executable
add_executable(todolist
    main.cpp
    todo_item.cpp
    database.cpp
    todo_repository.cpp
    command_parser.cpp
    formatter.cpp
    cli_handler.cpp
)

# Include directories
target_include_directories(todolist
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${SQLite3_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(todolist
    PRIVATE
        SQLite::SQLite3
)

# Set output directory
set_target_properties(todolist PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
