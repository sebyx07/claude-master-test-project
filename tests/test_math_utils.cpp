#include <gtest/gtest.h>
#include "todolist/math_utils.h"

using namespace todolist;

class MathUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Nothing to set up for these tests
    }
};

// Tests for add function

TEST_F(MathUtilsTest, AddPositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(10, 20), 30);
    EXPECT_EQ(add(100, 250), 350);
}

TEST_F(MathUtilsTest, AddNegativeNumbers) {
    EXPECT_EQ(add(-5, -3), -8);
    EXPECT_EQ(add(-10, -20), -30);
}

TEST_F(MathUtilsTest, AddMixedSignNumbers) {
    EXPECT_EQ(add(-5, 10), 5);
    EXPECT_EQ(add(15, -7), 8);
    EXPECT_EQ(add(-20, 20), 0);
}

TEST_F(MathUtilsTest, AddWithZero) {
    EXPECT_EQ(add(0, 0), 0);
    EXPECT_EQ(add(5, 0), 5);
    EXPECT_EQ(add(0, 7), 7);
}

// Tests for subtract function

TEST_F(MathUtilsTest, SubtractPositiveNumbers) {
    EXPECT_EQ(subtract(10, 3), 7);
    EXPECT_EQ(subtract(50, 20), 30);
    EXPECT_EQ(subtract(5, 5), 0);
}

TEST_F(MathUtilsTest, SubtractNegativeNumbers) {
    EXPECT_EQ(subtract(-5, -3), -2);
    EXPECT_EQ(subtract(-10, -20), 10);
}

TEST_F(MathUtilsTest, SubtractMixedSignNumbers) {
    EXPECT_EQ(subtract(10, -5), 15);
    EXPECT_EQ(subtract(-15, 7), -22);
}

TEST_F(MathUtilsTest, SubtractWithZero) {
    EXPECT_EQ(subtract(0, 0), 0);
    EXPECT_EQ(subtract(10, 0), 10);
    EXPECT_EQ(subtract(0, 10), -10);
}

// Tests for multiply function

TEST_F(MathUtilsTest, MultiplyPositiveNumbers) {
    EXPECT_EQ(multiply(3, 4), 12);
    EXPECT_EQ(multiply(7, 8), 56);
    EXPECT_EQ(multiply(10, 10), 100);
}

TEST_F(MathUtilsTest, MultiplyNegativeNumbers) {
    EXPECT_EQ(multiply(-3, -4), 12);
    EXPECT_EQ(multiply(-5, -7), 35);
}

TEST_F(MathUtilsTest, MultiplyMixedSignNumbers) {
    EXPECT_EQ(multiply(-5, 6), -30);
    EXPECT_EQ(multiply(8, -3), -24);
}

TEST_F(MathUtilsTest, MultiplyWithZero) {
    EXPECT_EQ(multiply(0, 0), 0);
    EXPECT_EQ(multiply(5, 0), 0);
    EXPECT_EQ(multiply(0, 7), 0);
    EXPECT_EQ(multiply(-3, 0), 0);
}

TEST_F(MathUtilsTest, MultiplyByOne) {
    EXPECT_EQ(multiply(5, 1), 5);
    EXPECT_EQ(multiply(1, 7), 7);
    EXPECT_EQ(multiply(-4, 1), -4);
}

// Tests for divide function

TEST_F(MathUtilsTest, DividePositiveNumbers) {
    EXPECT_EQ(divide(10, 2), 5);
    EXPECT_EQ(divide(20, 4), 5);
    EXPECT_EQ(divide(100, 10), 10);
}

TEST_F(MathUtilsTest, DivideNegativeNumbers) {
    EXPECT_EQ(divide(-10, -2), 5);
    EXPECT_EQ(divide(-20, -4), 5);
}

TEST_F(MathUtilsTest, DivideMixedSignNumbers) {
    EXPECT_EQ(divide(-10, 2), -5);
    EXPECT_EQ(divide(15, -3), -5);
}

TEST_F(MathUtilsTest, DivideByOne) {
    EXPECT_EQ(divide(7, 1), 7);
    EXPECT_EQ(divide(-9, 1), -9);
    EXPECT_EQ(divide(0, 1), 0);
}

TEST_F(MathUtilsTest, DivideZeroByNumber) {
    EXPECT_EQ(divide(0, 5), 0);
    EXPECT_EQ(divide(0, -3), 0);
}

TEST_F(MathUtilsTest, DivideByZeroThrowsException) {
    EXPECT_THROW(divide(10, 0), std::invalid_argument);
    EXPECT_THROW(divide(-5, 0), std::invalid_argument);
    EXPECT_THROW(divide(0, 0), std::invalid_argument);
}

TEST_F(MathUtilsTest, DivideIntegerDivision) {
    EXPECT_EQ(divide(7, 2), 3);  // Integer division
    EXPECT_EQ(divide(10, 3), 3);
    EXPECT_EQ(divide(-7, 2), -3);
}
