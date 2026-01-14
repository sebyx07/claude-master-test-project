/**
 * @file hello_world.h
 * @brief Simple hello world function for testing
 *
 * This file contains a simple function to demonstrate basic functionality
 * and testing patterns in the Todo List CLI application.
 */

#ifndef TODOLIST_HELLO_WORLD_H
#define TODOLIST_HELLO_WORLD_H

#include <string>

namespace todolist {

/**
 * @brief Print a hello world message
 * @return A greeting string
 *
 * Returns a simple hello world message that can be used for testing
 * and demonstration purposes.
 */
std::string printHelloWorld();

/**
 * @brief Generate a personalized greeting
 * @param name The name to greet
 * @return A personalized greeting string
 *
 * Returns a personalized greeting in the format "Hello, {name}!".
 * If the name is empty or contains only whitespace, returns "Hello, there!"
 */
std::string hello(const std::string& name);

} // namespace todolist

#endif // TODOLIST_HELLO_WORLD_H
