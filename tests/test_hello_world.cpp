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

// Tests for hello(name) function

TEST_F(HelloWorldTest, HelloBasicGreeting) {
    std::string result = hello("Alice");
    EXPECT_EQ(result, "Hello, Alice!");
}

TEST_F(HelloWorldTest, HelloEmptyString) {
    std::string result = hello("");
    EXPECT_EQ(result, "Hello, there!");
}

TEST_F(HelloWorldTest, HelloWhitespaceOnly) {
    std::string result = hello("   ");
    EXPECT_EQ(result, "Hello, there!");
}

TEST_F(HelloWorldTest, HelloLeadingWhitespace) {
    std::string result = hello("  Bob");
    EXPECT_EQ(result, "Hello, Bob!");
}

TEST_F(HelloWorldTest, HelloTrailingWhitespace) {
    std::string result = hello("Charlie  ");
    EXPECT_EQ(result, "Hello, Charlie!");
}

TEST_F(HelloWorldTest, HelloSurroundingWhitespace) {
    std::string result = hello("  Diana  ");
    EXPECT_EQ(result, "Hello, Diana!");
}

TEST_F(HelloWorldTest, HelloSpecialCharacters) {
    std::string result = hello("O'Brien");
    EXPECT_EQ(result, "Hello, O'Brien!");
}

TEST_F(HelloWorldTest, HelloHyphenatedName) {
    std::string result = hello("Jean-Luc");
    EXPECT_EQ(result, "Hello, Jean-Luc!");
}

TEST_F(HelloWorldTest, HelloWithNumbers) {
    std::string result = hello("Agent47");
    EXPECT_EQ(result, "Hello, Agent47!");
}

TEST_F(HelloWorldTest, HelloLongName) {
    std::string longName = "Christopher Alexander Montgomery III";
    std::string result = hello(longName);
    EXPECT_EQ(result, "Hello, " + longName + "!");
}

TEST_F(HelloWorldTest, HelloSingleCharacter) {
    std::string result = hello("X");
    EXPECT_EQ(result, "Hello, X!");
}

TEST_F(HelloWorldTest, HelloMixedCaseAndSpaces) {
    std::string result = hello("  Mary Jane  ");
    EXPECT_EQ(result, "Hello, Mary Jane!");
}
