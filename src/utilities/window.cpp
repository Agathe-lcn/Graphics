/**
 * @file window.cpp
 * @brief Implementation of window creation and OpenGL loading.
 */

#include "window.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>

/**
 * @brief Callback function for GLFW errors.
 * @param error Error code.
 * @param description Textual description of the error.
 */
static void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW error: " << description << " (" << error << ")" << std::endl;
}


GLFWwindow* initialize() {
    // Initialize the GLFW library
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configure OpenGL context settings
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(glfwErrorCallback);

    // Create the OS window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Floating Lantern", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Bind the OpenGL context to the window
    glfwMakeContextCurrent(window);
    // Load OpenGL function pointers using GLAD
    gladLoadGL();

    // Log the current version to ensure compliance with project requirements
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;

    return window;
}