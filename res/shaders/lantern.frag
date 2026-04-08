/**
 * @file lantern.frag
 * @brief Fragment shader for rendering paper lanterns.
 * This shader calculates the final color of each lantern by combining a base texture
 * with a dynamic flickering effect to simulate an internal flame.
 */

#version 400 core
out vec4 FragColor;

in vec2 TexCoord;
in float TimeOffset;    // Use to ensure that each lantern flickers out of sync with others

uniform sampler2D paperTexture;
uniform float time;

void main() {
    vec4 paper = texture(paperTexture, TexCoord);   // Sample the paper texture

    // Flicker calculation
    // Combines two sine waves at different frequencies for a pseudo-random effect
    float t = time + TimeOffset;
    float flicker = 0.7 + 0.3 * sin(t * 8.0);
    flicker *= 0.9 + 0.1 * sin(t * 15.0);

    // Warm flame colour overlay
    vec3 flameColor = vec3(1.0, 0.6, 0.3);

    vec3 finalColor = paper.rgb * flameColor * flicker;
    FragColor = vec4(finalColor, paper.a);  // Output with alpha transparency for the translucent paper effect
}
