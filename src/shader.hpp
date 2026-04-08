/**
 * @file shader.hpp
 * @brief Class for managing OpenGL shader compilation and uniform updates.
 */

#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    unsigned int ID;
    
    /**
     * @brief Constructor for the Shader class.
     * @param vertexPath Path to the vertex shader source file.
     * @param fragmentPath Path to the fragment shader source file. 
     */
    Shader(const char* vertexPath, const char* fragmentPath);

    /**
    * @brief Destructor for the Shader class. */
    ~Shader();
    
    /**
    * @brief Activates the shader program for rendering. 
    */
    void use() const;

    /**
     * @name Uniform Setters
     * Utility functions to pass data from CPU to GPU shaders
     * @
     */
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setMat4(const std::string &name, const glm::mat4 &value) const;
    /** @} */
    
private:
    /** 
     * @brief Checks for compilation or linking errors.
     * @param shader The shader or program ID to check.
     * @param type A string indicating "VERTEX", "FRAGMENT", or "PROGRAM". */
    void checkCompileErrors(unsigned int shader, const std::string& type);
};

#endif