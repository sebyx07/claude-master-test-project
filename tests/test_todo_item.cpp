#include <gtest/gtest.h>
#include "todolist/todo_item.h"
#include <chrono>
#include <thread>

using namespace todolist;

class TodoItemTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Nothing to set up for these tests
    }
};

TEST_F(TodoItemTest, DefaultConstructor) {
    TodoItem item;

    EXPECT_EQ(item.getId(), 0);
    EXPECT_EQ(item.getTitle(), "");
    EXPECT_EQ(item.getDescription(), "");
    EXPECT_FALSE(item.isCompleted());
    // created_at should be recent (within last second)
    auto now = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - item.getCreatedAt());
    EXPECT_LE(diff.count(), 1);
}

TEST_F(TodoItemTest, ConstructorWithTitleAndDescription) {
    TodoItem item("Buy groceries", "Milk, eggs, bread");

    EXPECT_EQ(item.getId(), 0);
    EXPECT_EQ(item.getTitle(), "Buy groceries");
    EXPECT_EQ(item.getDescription(), "Milk, eggs, bread");
    EXPECT_FALSE(item.isCompleted());
}

TEST_F(TodoItemTest, FullConstructor) {
    auto now = std::chrono::system_clock::now();
    TodoItem item(42, "Test task", "Test description", true, now);

    EXPECT_EQ(item.getId(), 42);
    EXPECT_EQ(item.getTitle(), "Test task");
    EXPECT_EQ(item.getDescription(), "Test description");
    EXPECT_TRUE(item.isCompleted());
    EXPECT_EQ(item.getCreatedAt(), now);
}

TEST_F(TodoItemTest, Setters) {
    TodoItem item;
    auto now = std::chrono::system_clock::now();

    item.setId(100);
    item.setTitle("New title");
    item.setDescription("New description");
    item.setCompleted(true);
    item.setCreatedAt(now);

    EXPECT_EQ(item.getId(), 100);
    EXPECT_EQ(item.getTitle(), "New title");
    EXPECT_EQ(item.getDescription(), "New description");
    EXPECT_TRUE(item.isCompleted());
    EXPECT_EQ(item.getCreatedAt(), now);
}

TEST_F(TodoItemTest, UnixTimeConversion) {
    // Use a known Unix timestamp: 2024-01-01 00:00:00 UTC = 1704067200
    std::time_t unix_time = 1704067200;
    auto time_point = TodoItem::fromUnixTime(unix_time);

    TodoItem item(1, "Test", "Description", false, time_point);

    EXPECT_EQ(item.getCreatedAtUnix(), unix_time);
}

TEST_F(TodoItemTest, FormattedTimestamp) {
    // Create a specific time point
    std::time_t unix_time = 1704067200; // 2024-01-01 00:00:00 UTC
    auto time_point = TodoItem::fromUnixTime(unix_time);

    TodoItem item(1, "Test", "Description", false, time_point);

    std::string formatted = item.getFormattedCreatedAt();

    // The exact format depends on timezone, but we can check it's not empty
    // and contains expected components
    EXPECT_FALSE(formatted.empty());
    EXPECT_TRUE(formatted.find("2024") != std::string::npos ||
                formatted.find("2023") != std::string::npos);
}

TEST_F(TodoItemTest, EmptyDescription) {
    TodoItem item("Task with no description", "");

    EXPECT_EQ(item.getTitle(), "Task with no description");
    EXPECT_EQ(item.getDescription(), "");
}

TEST_F(TodoItemTest, LongStrings) {
    std::string long_title(500, 'A');
    std::string long_description(5000, 'B');

    TodoItem item(long_title, long_description);

    EXPECT_EQ(item.getTitle(), long_title);
    EXPECT_EQ(item.getDescription(), long_description);
}
