#include "program.hpp"
#include "shader.hpp"
#include "lanternSystem.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void runProgram(GLFWwindow* window) {
    Shader lanternShader("../res/shaders/lantern.vert",
                         "../res/shaders/lantern.frag");

    LanternSystem lanterns(200, /*spread=*/15.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    float lastTime = static_cast<float>(glfwGetTime());

    const glm::vec3 cameraPos    = glm::vec3(0.0f,  3.0f, 23.0f);
    const glm::vec3 cameraTarget = glm::vec3(0.0f,  12.0f,  0.0f);
    const glm::vec3 cameraUp     = glm::vec3(0.0f,  1.0f,  0.0f);

    while (!glfwWindowShouldClose(window)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime   = currentTime - lastTime;
        lastTime          = currentTime;

        lanterns.update(deltaTime);

        // Night sky background colour
        glClearColor(0.05f, 0.05f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection = glm::perspective(
            glm::radians(60.0f),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f, 200.0f);

        lanterns.render(view, projection, lanternShader.ID, currentTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}