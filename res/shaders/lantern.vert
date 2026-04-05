#version 400 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in mat4 instanceModel;
layout (location = 6) in float instanceTimeOffset;

out vec2 TexCoord;
out float TimeOffset;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * instanceModel * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    TimeOffset = instanceTimeOffset;
}
