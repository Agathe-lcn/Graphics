#version 400 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D paperTexture;
uniform float time;

void main() {
    vec4 paper = texture(paperTexture, TexCoord);

    // Flicker effect
    float flicker = 0.7 + 0.3 * sin(time*8.0);
    flicker = flicker * (0.9 + 0.1 * sin(time*15.0));
    
    // Warm flame color overlay
    vec3 flameColor = vec3(1.0, 0.6, 0.3);
    
    vec3 finalColor = paper.rgb * flameColor * flicker;
    FragColor = vec4(finalColor, paper.a);
}