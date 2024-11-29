#version 300 es

precision mediump float;

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D textureSampler; // Amostrador de textura

void main() {
    // Amostra a cor da textura usando as coordenadas passadas do vertex shader
    FragColor = texture(textureSampler, TexCoord);
}
