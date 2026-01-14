#include <gtest/gtest.h>
#include "todolist/hello_world.h"

using namespace todolist;

class HelloWorldTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Nothing to set up for these tests
    }
};

TEST_F(HelloWorldTest, BasicFunctionality) {
    std::string result = printHelloWorld();
    EXPECT_EQ(result, "Hello from claude-task-master test!");
}

TEST_F(HelloWorldTest, ReturnsNonEmptyString) {
    std::string result = printHelloWorld();
    EXPECT_FALSE(result.empty());
}

TEST_F(HelloWorldTest, StringLength) {
    std::string result = printHelloWorld();
    EXPECT_EQ(result.length(), 35);
}

TEST_F(HelloWorldTest, StringContent) {
    std::string result = printHelloWorld();
    EXPECT_TRUE(result.find("claude-task-master") != std::string::npos);
    EXPECT_TRUE(result.find("Hello") != std::string::npos);
    EXPECT_TRUE(result.find("test!") != std::string::npos);
}
