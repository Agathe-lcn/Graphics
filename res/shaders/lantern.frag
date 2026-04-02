#version 400 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D paperTexture;
uniform float time;

void main() {
    vec4 paper = texture(paperTexture, TexCoord);
    float flicker = 0.7 + 0.3 * sin(time * 8.0);
    vec3 orange = vec3(1.0, 0.6, 0.3);
    FragColor = vec4(paper.rgb * orange * flicker, paper.a);
}