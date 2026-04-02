#include "utilities/window.hpp"
#include "program.hpp"
#include <GLFW/glfw3.h>

int main() {
    GLFWwindow* window = initialize();
    runProgram(window);
    glfwTerminate();
    return 0;
}