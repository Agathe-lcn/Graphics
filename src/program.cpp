#include "program.hpp"
#include "shader.hpp"
#include "lanternSystem.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstdlib>
#include <vector>

static const char* starVert = R"glsl(
#version 400 core
layout (location = 0) in vec3 aPos;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position  = projection * view * vec4(aPos, 1.0);
    gl_PointSize = 2.0;
}
)glsl";

static const char* starFrag = R"glsl(
#version 400 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 0.9);
}
)glsl";

static unsigned int compileStarShader() {
    unsigned int vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &starVert, nullptr);
    glCompileShader(vert);

    unsigned int frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &starFrag, nullptr);
    glCompileShader(frag);

    unsigned int prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    glDeleteShader(vert);
    glDeleteShader(frag);
    return prog;
}


static unsigned int createStarVAO(int count, float radius) {
    std::vector<float> positions;
    positions.reserve(count * 3);

    for (int i = 0; i < count; ++i) {
        float x, y, z, len;
        do {
            x = -1.0f + 2.0f * (static_cast<float>(std::rand()) / RAND_MAX);
            y = -1.0f + 2.0f * (static_cast<float>(std::rand()) / RAND_MAX);
            z = -1.0f + 2.0f * (static_cast<float>(std::rand()) / RAND_MAX);
            len = std::sqrt(x*x + y*y + z*z);
        } while (len < 0.0001f);

        positions.push_back(x / len * radius);
        positions.push_back(y/len*radius); // keep stars above horizon
        positions.push_back(z / len * radius);
    }

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(positions.size() * sizeof(float)),
                 positions.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    return VAO;
}

void runProgram(GLFWwindow* window) {
    Shader lanternShader("../res/shaders/lantern.vert",
                         "../res/shaders/lantern.frag");

    LanternSystem lanterns(200, 15.0f);

    // Stars
    const int    starCount  = 1500;
    unsigned int starShader = compileStarShader();
    unsigned int starVAO    = createStarVAO(starCount, 80.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    float lastTime = static_cast<float>(glfwGetTime());

    const glm::vec3 cameraPos    = glm::vec3(0.0f,  5.0f, 25.0f);
    const glm::vec3 cameraTarget = glm::vec3(0.0f,  8.0f,  0.0f);
    const glm::vec3 cameraUp     = glm::vec3(0.0f,  1.0f,  0.0f);

    while (!glfwWindowShouldClose(window)) {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime   = currentTime - lastTime;
        lastTime          = currentTime;

        lanterns.update(deltaTime);

        glClearColor(0.02f, 0.02f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection = glm::perspective(
            glm::radians(60.0f),
            static_cast<float>(width) / static_cast<float>(height),
            0.1f, 200.0f);

        // Draw stars
        glDepthMask(GL_FALSE);
        glUseProgram(starShader);
        glUniformMatrix4fv(glGetUniformLocation(starShader, "view"),       1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(starShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glBindVertexArray(starVAO);
        glDrawArrays(GL_POINTS, 0, starCount);
        glDepthMask(GL_TRUE);

        // Draw lanterns
        lanterns.render(view, projection, lanternShader.ID, currentTime);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(starShader);
}