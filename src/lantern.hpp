#ifndef LANTERN_HPP
#define LANTERN_HPP

#include <glm/glm.hpp>

class Lantern {
public:
    glm::vec3 position;
    float rotation;
    float flameIntensity;
    float speed;
    
    Lantern(glm::vec3 pos);
    ~Lantern();
    
    void update(float deltaTime);
    void render(const glm::mat4& view, const glm::mat4& projection, unsigned int shaderID);
    
private:
    unsigned int VAO, VBO, EBO;
    unsigned int textureID;
    void setupMesh();
    void setupTexture();
};

#endif