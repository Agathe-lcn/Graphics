#ifndef LANTERN_SYSTEM_HPP
#define LANTERN_SYSTEM_HPP

#include <glm/glm.hpp>
#include <vector>

class LanternSystem {
public:
    LanternSystem(int count, float spread = 12.0f);
    ~LanternSystem();

    void update(float deltaTime);

    void render(const glm::mat4& view,
                const glm::mat4& projection,
                unsigned int shaderID,
                float time);

private:
    struct Instance {
        glm::vec3 position;
        float     rotation;
        float     rotSpeed;
        float     ascentSpeed;
        float     driftPhase;   
        float     driftAmplitude; 
        float     timeOffset; 
        float     spawnY; 
    };

    std::vector<Instance> instances_;

    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int instanceVBO_;
    unsigned int textureID_;

    int instanceCount_;

    void setupGeometry();
    void setupTexture();

    void respawn(Instance& inst, float spread);
};

#endif // LANTERN_SYSTEM_HPP