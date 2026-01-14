# FindSQLite3.cmake
# Find SQLite3 library
#
# Sets the following variables:
#   SQLite3_FOUND - True if SQLite3 was found
#   SQLite3_INCLUDE_DIRS - Include directories
#   SQLite3_LIBRARIES - Libraries to link

find_path(SQLite3_INCLUDE_DIR
    NAMES sqlite3.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
)

find_library(SQLite3_LIBRARY
    NAMES sqlite3
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLite3
    REQUIRED_VARS SQLite3_LIBRARY SQLite3_INCLUDE_DIR
)

if(SQLite3_FOUND)
    set(SQLite3_LIBRARIES ${SQLite3_LIBRARY})
    set(SQLite3_INCLUDE_DIRS ${SQLite3_INCLUDE_DIR})

    if(NOT TARGET SQLite::SQLite3)
        add_library(SQLite::SQLite3 UNKNOWN IMPORTED)
        set_target_properties(SQLite::SQLite3 PROPERTIES
            IMPORTED_LOCATION "${SQLite3_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${SQLite3_INCLUDE_DIR}"
        )
    endif()
endif()

mark_as_advanced(SQLite3_INCLUDE_DIR SQLite3_LIBRARY)
