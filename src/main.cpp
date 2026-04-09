/**
 * @file main.cpp
 * @brief Entry point of the Floating Lanterns project.
 * This file coordinates the initialization and execution of the main program.
 */

#include "utilities/window.hpp"
#include "program.hpp"
#include <GLFW/glfw3.h>

/**
 * @brief Main function.
 * @return 0 on successful execution. 
 */
int main() {
    // 1. Setup OpenGL context and create the window
    GLFWwindow* window = initialize();

    // 2. Launch the main simulation and rendering loop
    runProgram(window);

    // 3. Clean up and exit
    glfwTerminate();
    
    return 0;
}