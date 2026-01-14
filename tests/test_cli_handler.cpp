#include <gtest/gtest.h>
#include "todolist/cli_handler.h"
#include "todolist/database.h"
#include "todolist/exceptions.h"
#include <memory>

using namespace todolist;

class CliHandlerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Use in-memory database for testing
        database = std::make_unique<Database>(":memory:");
        repository = std::make_unique<TodoRepository>(*database);

        // Create formatter without colors for testing
        auto formatter = std::make_unique<Formatter>(false);
        handler = std::make_unique<CliHandler>(*repository, std::move(formatter));
    }

    std::unique_ptr<Database> database;
    std::unique_ptr<TodoRepository> repository;
    std::unique_ptr<CliHandler> handler;
};

// Test handleAdd
TEST_F(CliHandlerTest, HandleAddWithTitleOnly) {
    std::vector<std::string> args = {"Buy groceries"};
    std::string result = handler->handleAdd(args);

    EXPECT_NE(result.find("created successfully"), std::string::npos);

    auto items = repository->findAll();
    ASSERT_EQ(items.size(), 1);
    EXPECT_EQ(items[0].getTitle(), "Buy groceries");
    EXPECT_TRUE(items[0].getDescription().empty());
}

TEST_F(CliHandlerTest, HandleAddWithTitleAndDescription) {
    std::vector<std::string> args = {"Buy groceries", "Milk, bread, eggs"};
    std::string result = handler->handleAdd(args);

    EXPECT_NE(result.find("created successfully"), std::string::npos);

    auto items = repository->findAll();
    ASSERT_EQ(items.size(), 1);
    EXPECT_EQ(items[0].getTitle(), "Buy groceries");
    EXPECT_EQ(items[0].getDescription(), "Milk, bread, eggs");
}

TEST_F(CliHandlerTest, HandleAddWithNoArgs) {
    std::vector<std::string> args = {};
    EXPECT_THROW(handler->handleAdd(args), ValidationException);
}

TEST_F(CliHandlerTest, HandleAddWithEmptyTitle) {
    std::vector<std::string> args = {""};
    EXPECT_THROW(handler->handleAdd(args), ValidationException);
}

// Test handleList
TEST_F(CliHandlerTest, HandleListEmpty) {
    std::vector<std::string> args = {};
    std::string result = handler->handleList(args);

    EXPECT_NE(result.find("No todo items found"), std::string::npos);
}

TEST_F(CliHandlerTest, HandleListAll) {
    // Add some items
    repository->create(TodoItem("Task 1", "Description 1"));
    repository->create(TodoItem("Task 2", "Description 2"));

    std::vector<std::string> args = {"all"};
    std::string result = handler->handleList(args);

    EXPECT_NE(result.find("Task 1"), std::string::npos);
    EXPECT_NE(result.find("Task 2"), std::string::npos);
}

TEST_F(CliHandlerTest, HandleListCompleted) {
    // Add some items
    auto item1 = TodoItem("Task 1", "");
    repository->create(item1);

    auto item2 = TodoItem("Task 2", "");
    item2.setCompleted(true);
    repository->create(item2);

    std::vector<std::string> args = {"completed"};
    std::string result = handler->handleList(args);

    EXPECT_EQ(result.find("Task 1"), std::string::npos);
    EXPECT_NE(result.find("Task 2"), std::string::npos);
}

TEST_F(CliHandlerTest, HandleListPending) {
    // Add some items
    auto item1 = TodoItem("Task 1", "");
    repository->create(item1);

    auto item2 = TodoItem("Task 2", "");
    item2.setCompleted(true);
    repository->create(item2);

    std::vector<std::string> args = {"pending"};
    std::string result = handler->handleList(args);

    EXPECT_NE(result.find("Task 1"), std::string::npos);
    EXPECT_EQ(result.find("Task 2"), std::string::npos);
}

TEST_F(CliHandlerTest, HandleListInvalidFilter) {
    std::vector<std::string> args = {"invalid"};
    EXPECT_THROW(handler->handleList(args), ValidationException);
}

// Test handleComplete
TEST_F(CliHandlerTest, HandleComplete) {
    auto item = TodoItem("Task 1", "");
    item = repository->create(item);
    int id = item.getId();

    std::vector<std::string> args = {std::to_string(id)};
    std::string result = handler->handleComplete(args);

    EXPECT_NE(result.find("marked as completed"), std::string::npos);

    auto updated = repository->findById(id);
    ASSERT_TRUE(updated.has_value());
    EXPECT_TRUE(updated->isCompleted());
}

TEST_F(CliHandlerTest, HandleCompleteNoArgs) {
    std::vector<std::string> args = {};
    EXPECT_THROW(handler->handleComplete(args), ValidationException);
}

TEST_F(CliHandlerTest, HandleCompleteInvalidId) {
    std::vector<std::string> args = {"invalid"};
    EXPECT_THROW(handler->handleComplete(args), ValidationException);
}

TEST_F(CliHandlerTest, HandleCompleteNonExistentId) {
    std::vector<std::string> args = {"999"};
    EXPECT_THROW(handler->handleComplete(args), NotFoundException);
}

TEST_F(CliHandlerTest, HandleCompleteAlreadyCompleted) {
    auto item = TodoItem("Task 1", "");
    item.setCompleted(true);
    item = repository->create(item);
    int id = item.getId();

    std::vector<std::string> args = {std::to_string(id)};
    EXPECT_THROW(handler->handleComplete(args), ValidationException);
}

// Test handleDelete
TEST_F(CliHandlerTest, HandleDelete) {
    auto item = TodoItem("Task 1", "");
    item = repository->create(item);
    int id = item.getId();

    std::vector<std::string> args = {std::to_string(id)};
    std::string result = handler->handleDelete(args);

    EXPECT_NE(result.find("deleted successfully"), std::string::npos);

    auto deleted = repository->findById(id);
    EXPECT_FALSE(deleted.has_value());
}

TEST_F(CliHandlerTest, HandleDeleteNoArgs) {
    std::vector<std::string> args = {};
    EXPECT_THROW(handler->handleDelete(args), ValidationException);
}

TEST_F(CliHandlerTest, HandleDeleteInvalidId) {
    std::vector<std::string> args = {"invalid"};
    EXPECT_THROW(handler->handleDelete(args), ValidationException);
}

TEST_F(CliHandlerTest, HandleDeleteNonExistentId) {
    std::vector<std::string> args = {"999"};
    EXPECT_THROW(handler->handleDelete(args), NotFoundException);
}

// Test handleSearch
TEST_F(CliHandlerTest, HandleSearch) {
    repository->create(TodoItem("Buy groceries", ""));
    repository->create(TodoItem("Buy books", ""));
    repository->create(TodoItem("Sell car", ""));

    std::vector<std::string> args = {"Buy"};
    std::string result = handler->handleSearch(args);

    EXPECT_NE(result.find("Buy groceries"), std::string::npos);
    EXPECT_NE(result.find("Buy books"), std::string::npos);
    EXPECT_EQ(result.find("Sell car"), std::string::npos);
}

TEST_F(CliHandlerTest, HandleSearchNoResults) {
    repository->create(TodoItem("Task 1", ""));

    std::vector<std::string> args = {"nonexistent"};
    std::string result = handler->handleSearch(args);

    EXPECT_NE(result.find("No todo items found"), std::string::npos);
}

TEST_F(CliHandlerTest, HandleSearchNoArgs) {
    std::vector<std::string> args = {};
    EXPECT_THROW(handler->handleSearch(args), ValidationException);
}

TEST_F(CliHandlerTest, HandleSearchEmptyQuery) {
    std::vector<std::string> args = {""};
    EXPECT_THROW(handler->handleSearch(args), ValidationException);
}

// Test handleHelp
TEST_F(CliHandlerTest, HandleHelpGeneral) {
    std::vector<std::string> args = {};
    std::string result = handler->handleHelp(args);

    EXPECT_NE(result.find("Usage"), std::string::npos);
    EXPECT_NE(result.find("add"), std::string::npos);
    EXPECT_NE(result.find("list"), std::string::npos);
}

TEST_F(CliHandlerTest, HandleHelpSpecificCommand) {
    std::vector<std::string> args = {"add"};
    std::string result = handler->handleHelp(args);

    EXPECT_NE(result.find("add"), std::string::npos);
    EXPECT_NE(result.find("title"), std::string::npos);
}

TEST_F(CliHandlerTest, HandleHelpInvalidCommand) {
    std::vector<std::string> args = {"invalid"};
    EXPECT_THROW(handler->handleHelp(args), ValidationException);
}

// Test handleVersion
TEST_F(CliHandlerTest, HandleVersion) {
    std::string result = handler->handleVersion();

    EXPECT_NE(result.find("Version"), std::string::npos);
}

// Test execute method
TEST_F(CliHandlerTest, ExecuteAddCommand) {
    ParsedCommand cmd;
    cmd.command = Command::ADD;
    cmd.args = {"Test task"};

    int exitCode = handler->execute(cmd);
    EXPECT_EQ(exitCode, 0);

    auto items = repository->findAll();
    EXPECT_EQ(items.size(), 1);
}

TEST_F(CliHandlerTest, ExecuteUnknownCommand) {
    ParsedCommand cmd;
    cmd.command = Command::UNKNOWN;

    int exitCode = handler->execute(cmd);
    EXPECT_EQ(exitCode, 1);
}

TEST_F(CliHandlerTest, ExecuteValidationError) {
    ParsedCommand cmd;
    cmd.command = Command::ADD;
    cmd.args = {}; // Missing required title

    int exitCode = handler->execute(cmd);
    EXPECT_EQ(exitCode, 1);
}

TEST_F(CliHandlerTest, ExecuteNotFoundError) {
    ParsedCommand cmd;
    cmd.command = Command::COMPLETE;
    cmd.args = {"999"}; // Non-existent ID

    int exitCode = handler->execute(cmd);
    EXPECT_EQ(exitCode, 1);
}

// Test ID parsing
TEST_F(CliHandlerTest, ParseIdValid) {
    std::vector<std::string> args = {"42"};
    // This is tested indirectly through handleComplete/handleDelete

    auto item = TodoItem("Test", "");
    item = repository->create(item);
    int id = item.getId();

    args = {std::to_string(id)};
    EXPECT_NO_THROW(handler->handleComplete(args));
}

TEST_F(CliHandlerTest, ParseIdNegative) {
    std::vector<std::string> args = {"-1"};
    EXPECT_THROW(handler->handleComplete(args), ValidationException);
}

TEST_F(CliHandlerTest, ParseIdZero) {
    std::vector<std::string> args = {"0"};
    EXPECT_THROW(handler->handleComplete(args), ValidationException);
}

TEST_F(CliHandlerTest, ParseIdWithExtraCharacters) {
    std::vector<std::string> args = {"42abc"};
    EXPECT_THROW(handler->handleComplete(args), ValidationException);
}
