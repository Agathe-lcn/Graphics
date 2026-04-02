#include "program.hpp"
#include "shader.hpp"
#include "lantern.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

void runProgram(GLFWwindow* window) {
    Shader lanternShader("../res/shaders/lantern.vert", "../res/shaders/lantern.frag");
    
    Lantern lantern(glm::vec3(0.0f, 0.0f, -5.0f));
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    
    float lastTime = glfwGetTime();
    
    glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 8.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    while (!glfwWindowShouldClose(window)) {
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        
        lantern.update(deltaTime);
        
        glClearColor(0.15f, 0.15f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
        
        lantern.render(view, projection, lanternShader.ID);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}