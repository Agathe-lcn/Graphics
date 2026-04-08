/**
 * @file window.hpp
 * @brief Window management and OpenGL context initialization header.
 */

#ifndef WINDOW_HPP
#define WINDOW_HPP

/**
 * @brief Forward declaration of the GLFWwindow structure. 
 */
struct GLFWwindow;

/**
 * @brief Initializes the window and OpenGL context.
 * @return A pointer to the created GLFWwindow object.
 * @note Configures OpenGL to version 4.0 Core Profile.
 */
GLFWwindow* initialize();

#endif