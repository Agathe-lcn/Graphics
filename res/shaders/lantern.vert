/**
 * @file lantern.vert
 * @brief Vertex shader for instanced rendering of paper lanterns.
 * This shader handles the transformation of each lantern instance from local space 
 * to clip space, and passes necessary data to the fragment shader for texturing and animation.
 */

#version 400 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 instanceModel;
layout (location = 6) in float instanceTimeOffset;  // Used to randomize the flickering effect in the fragment shader

out vec2 TexCoord;
out float TimeOffset;

uniform mat4 view;
uniform mat4 projection;

void main() {
    // The final vertex position is calculated by multiplying the projection, view, and instance-specific model matrices.
    // This allows hundreds of lanterns to be rendered in unique positions with a single draw call.
    gl_Position = projection * view * instanceModel * vec4(aPos, 1.0);
    
    TexCoord = aTexCoord;
    TimeOffset = instanceTimeOffset;
}
