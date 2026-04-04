#version 400 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D paperTexture;
uniform float time;

void main() {
    vec4 paper = texture(paperTexture, TexCoord);

    // Realistic flicker
    float flicker1 = 0.7 + 0.3 * sin(time*8.0);
    float flicker2 = 0.9 + 0.1 * sin(time*15.0);
    float flicker3 = 0.95 + 0.05 * sin(time*25.0);
    float flicker = flicker1 * flicker2 * flicker3;

    // Flame color with subtle variations
    float r = 1.0;
    float g = 0.5 + 0.1 * sin(time*10.0);
    float b = 0.2 + 0.05 * sin(time*12.0);
    vec3 flameColor = vec3(r,g,b);

    vec3 finalColor = vec3(r,g,b);
    FragColor = vec4(finalColor, paper.a);
}