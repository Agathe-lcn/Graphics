/**
 * @file lanternSystem.cpp
 * @brief Implementation of the lantern simulation and instanced rendering logic. */

#include "lanternSystem.hpp"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>

static const float kVertices[] = {
    // positions         // tex coords
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

     0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f
};

static const unsigned int kIndices[] = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16,
    20, 21, 22, 22, 23, 20
};

static float randF(float lo, float hi) {
    return lo + (hi - lo) * (static_cast<float>(std::rand()) / RAND_MAX);
}

LanternSystem::LanternSystem(int count, float spread)
    : instanceCount_(count)
{
    instances_.reserve(count);
    for (int i = 0; i < count; ++i) {
        Instance inst{};

        inst.position = glm::vec3(randF(-spread, spread), randF(-5.0f, 40.0f), randF(-spread, spread));
        inst.rotation = randF(0.0f, 6.28f);
        inst.rotSpeed = randF(0.2f, 0.8f) * (std::rand() % 2 ? 1.f : -1.f);
        inst.ascentSpeed = randF(1.0f, 3.5f);
        inst.driftPhase = randF(0.0f, 6.28f);
        inst.driftAmplitude = randF(0.005f, 0.015f);
        inst.timeOffset = randF(0.0f, 100.0f);
        inst.spawnY = -8.0f; 
        instances_.push_back(inst);
    }

    setupGeometry();
    setupTexture();
}

LanternSystem::~LanternSystem() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
    glDeleteBuffers(1, &instanceVBO_);
    glDeleteTextures(1, &textureID_);
}

void LanternSystem::setupGeometry() {
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);
    glGenBuffers(1, &instanceVBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices, GL_STATIC_DRAW);

    // location 0 : position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // location 1 : texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    const std::size_t stride = sizeof(glm::mat4) + sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_);
    glBufferData(GL_ARRAY_BUFFER, instanceCount_ * static_cast<int>(stride), nullptr, GL_DYNAMIC_DRAW);

    for (int col = 0; col < 4; ++col) {
        GLuint loc = 2 + col; // locations 2, 3, 4, 5
        glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), reinterpret_cast<void*>(col * sizeof(glm::vec4)));
        glEnableVertexAttribArray(loc);
        glVertexAttribDivisor(loc, 1);
    }

    // location 6 : per-instance time offset
    glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), reinterpret_cast<void*>(sizeof(glm::mat4)));
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);
}

/**
 * @brief Generate a procedural 512x512 texture for the lanterns.
 * @details Creates a paper-like appearance with dark borders and horizontal/vertical
 * bands without using external image files. 
 */
void LanternSystem::setupTexture() {
    const int size = 512;
    std::vector<unsigned char> data(size * size * 4);

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            int i = (y * size + x) * 4;
            float u = static_cast<float>(x) / size;
            float v = static_cast<float>(y) / size;

            int r = 255, g = 165, b = 0;

            bool vStripe = (u > 0.20f && u < 0.25f) || (u > 0.40f && u < 0.45f) || (u > 0.60f && u < 0.65f) || (u > 0.80f && u < 0.85f);
            bool hBand = v < 0.15f || v > 0.85f || (v > 0.45f && v < 0.55f);
            bool border = u < 0.05f || u > 0.95f || v < 0.05f || v > 0.95f;

            if (border)
                { r = 80;  g = 40;  b = 20; }
            else if (vStripe || hBand) 
                { r = 139; g = 69;  b = 19; }

            data[i+0] = static_cast<unsigned char>(r);
            data[i+1] = static_cast<unsigned char>(g);
            data[i+2] = static_cast<unsigned char>(b);
            data[i+3] = 220;
        }
    }

    glGenTextures(1, &textureID_);
    glBindTexture(GL_TEXTURE_2D, textureID_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void LanternSystem::respawn(Instance& inst, float spread) {
    inst.position = glm::vec3(randF(-spread, spread), inst.spawnY, randF(-spread, spread));
    inst.rotation = randF(0.0f, 6.28f);
    inst.rotSpeed = randF(0.2f, 0.8f) * (std::rand() % 2 ? 1.f : -1.f);
    inst.ascentSpeed = randF(1.0f, 3.5f);
    inst.driftPhase = randF(0.0f, 6.28f);
    inst.driftAmplitude = randF(0.005f, 0.015f);
    inst.timeOffset = randF(0.0f, 100.0f);
}

/**
 * @brief Updates lantern positions and recycles them if they go above a certain height.
 * @param deltaTime Time elapsed since the last frame. */
void LanternSystem::update(float deltaTime) {
    const float kCullY = 60.0f; // recycle when this high
    const float spread = 12.0f;

    for (auto& inst : instances_) {
        inst.position.y += inst.ascentSpeed * deltaTime;
        inst.position.x += std::sin(inst.position.y * 2.0f + inst.driftPhase) * inst.driftAmplitude;
        inst.position.z += std::cos(inst.position.y * 2.0f + inst.driftPhase) * inst.driftAmplitude;
        inst.rotation += inst.rotSpeed * deltaTime;

        // When a lantern rises out of view, send it back to the bottom.
        if (inst.position.y > kCullY)
            respawn(inst, spread);
    }
}

/** 
 * @brief Orchestrates the instanced drawing process.
 * @details
 * 1. Calculates model matrices for all instances.
 * 2. Uploads instance data to the GPU.
 * 3. Executes a single instanced draw call for all lanterns. 
 */
void LanternSystem::render(const glm::mat4& view, const glm::mat4& projection, unsigned int shaderID, float time)
{
    const std::size_t stride = sizeof(glm::mat4) + sizeof(float);
    std::vector<unsigned char> buffer(instanceCount_ * stride);

    for (int i = 0; i < instanceCount_; ++i) {
        const Instance& inst = instances_[i];

        glm::mat4 model = glm::translate(glm::mat4(1.0f), inst.position);
        model = glm::rotate(model, inst.rotation, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 1.2f, 0.8f)); // slightly taller

        unsigned char* dst = buffer.data() + i * stride;
        std::memcpy(dst, glm::value_ptr(model), sizeof(glm::mat4));
        std::memcpy(dst + sizeof(glm::mat4), &inst.timeOffset, sizeof(float));
    }

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(buffer.size()), buffer.data());

    glUseProgram(shaderID);
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform1f(glGetUniformLocation(shaderID, "time"), time);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID_);
    glUniform1i(glGetUniformLocation(shaderID, "paperTexture"), 0);

    glBindVertexArray(VAO_);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr, instanceCount_);
    glBindVertexArray(0);
}