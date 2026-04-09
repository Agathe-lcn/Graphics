/**
 * @file lanternSystem.hpp
 * @brief Manages a collection of animated lanterns using instancing
 */

#ifndef LANTERN_SYSTEM_HPP
#define LANTERN_SYSTEM_HPP

#include <glm/glm.hpp>
#include <vector>

class LanternSystem {
public:
    /**
     * @brief Constructor for the lantern system.
     * @param count Number of lanterns to simulate and render.
     * @param spread Horizontal range for lantern positioning. 
     */
    LanternSystem(int count, float spread = 12.0f);

    /**
     * @brief Destructor for the lantern system. 
     */
    ~LanternSystem();

    /**
     * @brief Updates the physics and animation of all lanterns.
     */
    void update(float deltaTime);

    /**
     * @brief Renders all lanterns using instancing.
     */
    void render(const glm::mat4& view, const glm::mat4& projection, unsigned int shaderID, float time);

private:
    /**
     * @brief Internal structure to store simulation data for each lantern instance. 
     */
    struct Instance {
        glm::vec3 position;
        float rotation;
        float rotSpeed;
        float ascentSpeed;
        float driftPhase;   ///< Phase offset for the swaying motion
        float driftAmplitude;   ///< Strength of the swaying motion
        float timeOffset;   ///< Offset for the flicker shader effect
        float spawnY;   ///< Height at which lanterns are recycled
    };

    std::vector<Instance> instances_;

    unsigned int VAO_;
    unsigned int VBO_;
    unsigned int EBO_;
    unsigned int instanceVBO_;
    unsigned int textureID_;

    int instanceCount_;

    void setupGeometry();   ///< Sets up the cube mesh and instance buffers
    void setupTexture();    ///< Procedurally generates the paper texture
    void respawn(Instance& inst, float spread);     ///< Resets a lantern's properties
};

#endif // LANTERN_SYSTEM_HPP