/**
 * @file math_utils.h
 * @brief Simple math utility functions for testing
 *
 * This file contains basic arithmetic functions to demonstrate testing
 * patterns and edge case handling in the Todo List CLI application.
 */

#ifndef TODOLIST_MATH_UTILS_H
#define TODOLIST_MATH_UTILS_H

namespace todolist {

/**
 * @brief Add two integers
 * @param a First integer
 * @param b Second integer
 * @return The sum of a and b
 *
 * Returns the sum of two integers. This function demonstrates
 * basic arithmetic operations for testing purposes.
 */
int add(int a, int b);

/**
 * @brief Subtract two integers
 * @param a First integer
 * @param b Second integer
 * @return The difference of a and b (a - b)
 *
 * Returns the difference between two integers. This function demonstrates
 * basic arithmetic operations for testing purposes.
 */
int subtract(int a, int b);

/**
 * @brief Multiply two integers
 * @param a First integer
 * @param b Second integer
 * @return The product of a and b
 *
 * Returns the product of two integers. This function demonstrates
 * basic arithmetic operations for testing purposes.
 */
int multiply(int a, int b);

/**
 * @brief Divide two integers
 * @param a Numerator
 * @param b Denominator
 * @return The quotient of a divided by b
 * @throws std::invalid_argument if b is zero
 *
 * Returns the quotient of two integers using integer division.
 * Throws an exception if division by zero is attempted.
 */
int divide(int a, int b);

} // namespace todolist

#endif // TODOLIST_MATH_UTILS_H
