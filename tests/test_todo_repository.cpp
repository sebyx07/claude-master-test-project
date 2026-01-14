#include <gtest/gtest.h>
#include "todolist/todo_repository.h"
#include "todolist/database.h"

using namespace todolist;

class TodoRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Use in-memory database for testing
        db_ = std::make_unique<Database>(":memory:");
        repo_ = std::make_unique<TodoRepository>(*db_);
    }

    void TearDown() override {
        repo_.reset();
        db_.reset();
    }

    std::unique_ptr<Database> db_;
    std::unique_ptr<TodoRepository> repo_;
};

TEST_F(TodoRepositoryTest, CreateTodoItem) {
    TodoItem item("Buy groceries", "Milk and eggs");

    TodoItem created = repo_->create(item);

    EXPECT_GT(created.getId(), 0);
    EXPECT_EQ(created.getTitle(), "Buy groceries");
    EXPECT_EQ(created.getDescription(), "Milk and eggs");
    EXPECT_FALSE(created.isCompleted());
}

TEST_F(TodoRepositoryTest, FindById) {
    TodoItem item("Test task", "Test description");
    TodoItem created = repo_->create(item);

    auto found = repo_->findById(created.getId());

    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getId(), created.getId());
    EXPECT_EQ(found->getTitle(), "Test task");
    EXPECT_EQ(found->getDescription(), "Test description");
    EXPECT_FALSE(found->isCompleted());
}

TEST_F(TodoRepositoryTest, FindByIdNotFound) {
    auto found = repo_->findById(9999);

    EXPECT_FALSE(found.has_value());
}

TEST_F(TodoRepositoryTest, FindAll) {
    repo_->create(TodoItem("Task 1", "Description 1"));
    repo_->create(TodoItem("Task 2", "Description 2"));
    repo_->create(TodoItem("Task 3", "Description 3"));

    auto items = repo_->findAll();

    EXPECT_EQ(items.size(), 3);
}

TEST_F(TodoRepositoryTest, FindAllEmpty) {
    auto items = repo_->findAll();

    EXPECT_TRUE(items.empty());
}

TEST_F(TodoRepositoryTest, FindCompleted) {
    TodoItem item1("Task 1", "Desc 1");
    TodoItem item2("Task 2", "Desc 2");
    TodoItem item3("Task 3", "Desc 3");

    auto created1 = repo_->create(item1);
    auto created2 = repo_->create(item2);
    repo_->create(item3);

    // Mark first two as completed
    created1.setCompleted(true);
    created2.setCompleted(true);
    repo_->update(created1);
    repo_->update(created2);

    auto completed = repo_->findCompleted();

    EXPECT_EQ(completed.size(), 2);
    for (const auto& item : completed) {
        EXPECT_TRUE(item.isCompleted());
    }
}

TEST_F(TodoRepositoryTest, FindPending) {
    TodoItem item1("Task 1", "Desc 1");
    TodoItem item2("Task 2", "Desc 2");
    TodoItem item3("Task 3", "Desc 3");

    auto created1 = repo_->create(item1);
    repo_->create(item2);
    repo_->create(item3);

    // Mark only first one as completed
    created1.setCompleted(true);
    repo_->update(created1);

    auto pending = repo_->findPending();

    EXPECT_EQ(pending.size(), 2);
    for (const auto& item : pending) {
        EXPECT_FALSE(item.isCompleted());
    }
}

TEST_F(TodoRepositoryTest, FindByTitle) {
    repo_->create(TodoItem("Buy groceries", "Food items"));
    repo_->create(TodoItem("Buy books", "Programming books"));
    repo_->create(TodoItem("Clean house", "Living room"));

    auto results = repo_->findByTitle("Buy");

    EXPECT_EQ(results.size(), 2);
    for (const auto& item : results) {
        EXPECT_TRUE(item.getTitle().find("Buy") != std::string::npos);
    }
}

TEST_F(TodoRepositoryTest, FindByTitleNoMatch) {
    repo_->create(TodoItem("Task 1", "Desc 1"));

    auto results = repo_->findByTitle("NonExistent");

    EXPECT_TRUE(results.empty());
}

TEST_F(TodoRepositoryTest, FindByTitleCaseInsensitive) {
    repo_->create(TodoItem("Buy Groceries", "Food"));

    auto results = repo_->findByTitle("buy");

    EXPECT_EQ(results.size(), 1);
}

TEST_F(TodoRepositoryTest, UpdateTodoItem) {
    TodoItem item("Original title", "Original description");
    auto created = repo_->create(item);

    created.setTitle("Updated title");
    created.setDescription("Updated description");
    created.setCompleted(true);

    bool updated = repo_->update(created);

    EXPECT_TRUE(updated);

    auto found = repo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getTitle(), "Updated title");
    EXPECT_EQ(found->getDescription(), "Updated description");
    EXPECT_TRUE(found->isCompleted());
}

TEST_F(TodoRepositoryTest, UpdateNonExistent) {
    TodoItem item(9999, "Title", "Description", false, std::chrono::system_clock::now());

    bool updated = repo_->update(item);

    EXPECT_FALSE(updated);
}

TEST_F(TodoRepositoryTest, RemoveTodoItem) {
    TodoItem item("Task to delete", "Will be removed");
    auto created = repo_->create(item);

    bool removed = repo_->remove(created.getId());

    EXPECT_TRUE(removed);

    auto found = repo_->findById(created.getId());
    EXPECT_FALSE(found.has_value());
}

TEST_F(TodoRepositoryTest, RemoveNonExistent) {
    bool removed = repo_->remove(9999);

    EXPECT_FALSE(removed);
}

TEST_F(TodoRepositoryTest, Count) {
    EXPECT_EQ(repo_->count(), 0);

    repo_->create(TodoItem("Task 1", "Desc 1"));
    repo_->create(TodoItem("Task 2", "Desc 2"));
    repo_->create(TodoItem("Task 3", "Desc 3"));

    EXPECT_EQ(repo_->count(), 3);
}

TEST_F(TodoRepositoryTest, CountCompleted) {
    auto item1 = repo_->create(TodoItem("Task 1", "Desc 1"));
    auto item2 = repo_->create(TodoItem("Task 2", "Desc 2"));
    repo_->create(TodoItem("Task 3", "Desc 3"));

    EXPECT_EQ(repo_->countCompleted(), 0);

    item1.setCompleted(true);
    item2.setCompleted(true);
    repo_->update(item1);
    repo_->update(item2);

    EXPECT_EQ(repo_->countCompleted(), 2);
}

TEST_F(TodoRepositoryTest, CountPending) {
    auto item1 = repo_->create(TodoItem("Task 1", "Desc 1"));
    repo_->create(TodoItem("Task 2", "Desc 2"));
    repo_->create(TodoItem("Task 3", "Desc 3"));

    EXPECT_EQ(repo_->countPending(), 3);

    item1.setCompleted(true);
    repo_->update(item1);

    EXPECT_EQ(repo_->countPending(), 2);
}

TEST_F(TodoRepositoryTest, MultipleCreatesAndReads) {
    // Create many items
    for (int i = 0; i < 100; ++i) {
        repo_->create(TodoItem("Task " + std::to_string(i), "Description " + std::to_string(i)));
    }

    EXPECT_EQ(repo_->count(), 100);

    auto all_items = repo_->findAll();
    EXPECT_EQ(all_items.size(), 100);
}

TEST_F(TodoRepositoryTest, EmptyDescription) {
    TodoItem item("Task with no description", "");
    auto created = repo_->create(item);

    auto found = repo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getDescription(), "");
}
