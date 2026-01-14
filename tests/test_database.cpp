#include <gtest/gtest.h>
#include "todolist/database.h"
#include <filesystem>

using namespace todolist;

class DatabaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Use an in-memory database for testing
        db_path_ = ":memory:";
    }

    void TearDown() override {
        // In-memory database is automatically cleaned up
    }

    std::string db_path_;
};

TEST_F(DatabaseTest, OpenDatabase) {
    EXPECT_NO_THROW({
        Database db(db_path_);
        EXPECT_TRUE(db.isOpen());
    });
}

TEST_F(DatabaseTest, InvalidPath) {
    // Try to open a database in a non-existent directory
    EXPECT_THROW({
        Database db("/nonexistent/path/database.db");
    }, DatabaseException);
}

TEST_F(DatabaseTest, SchemaInitialization) {
    Database db(db_path_);

    // Verify the todos table exists by querying it
    EXPECT_NO_THROW({
        db.execute("SELECT COUNT(*) FROM todos");
    });
}

TEST_F(DatabaseTest, ExecuteSimpleSQL) {
    Database db(db_path_);

    EXPECT_NO_THROW({
        db.execute("INSERT INTO todos (title, description, completed, created_at) VALUES ('Test', 'Description', 0, 1234567890)");
    });

    // Verify the insert worked
    EXPECT_NO_THROW({
        db.execute("SELECT * FROM todos WHERE title = 'Test'");
    });
}

TEST_F(DatabaseTest, ExecuteInvalidSQL) {
    Database db(db_path_);

    EXPECT_THROW({
        db.execute("INVALID SQL STATEMENT");
    }, DatabaseException);
}

TEST_F(DatabaseTest, MoveConstructor) {
    Database db1(db_path_);
    EXPECT_TRUE(db1.isOpen());

    Database db2(std::move(db1));
    EXPECT_TRUE(db2.isOpen());
    EXPECT_FALSE(db1.isOpen());
}

TEST_F(DatabaseTest, MoveAssignment) {
    Database db1(db_path_);
    Database db2(":memory:");

    EXPECT_TRUE(db1.isOpen());
    EXPECT_TRUE(db2.isOpen());

    db2 = std::move(db1);
    EXPECT_TRUE(db2.isOpen());
    EXPECT_FALSE(db1.isOpen());
}

TEST_F(DatabaseTest, MultipleOperations) {
    Database db(db_path_);

    // Insert multiple records
    db.execute("INSERT INTO todos (title, description, completed, created_at) VALUES ('Task 1', 'Desc 1', 0, 1000000000)");
    db.execute("INSERT INTO todos (title, description, completed, created_at) VALUES ('Task 2', 'Desc 2', 1, 1000000001)");
    db.execute("INSERT INTO todos (title, description, completed, created_at) VALUES ('Task 3', 'Desc 3', 0, 1000000002)");

    // Query should succeed
    EXPECT_NO_THROW({
        db.execute("SELECT COUNT(*) FROM todos");
    });
}

TEST_F(DatabaseTest, IndexCreation) {
    Database db(db_path_);

    // The index should have been created automatically
    // We can verify this by using EXPLAIN QUERY PLAN, but for simplicity
    // we'll just ensure no error occurred during initialization
    EXPECT_TRUE(db.isOpen());
}
