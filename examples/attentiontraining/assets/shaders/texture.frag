#version 450 core

in vec2 fragTexCoord;      // Coordenadas de textura passadas pelo vertex shader
out vec4 outColor;         // Cor de saída para o fragment shader

uniform sampler2D textureSampler; // Amostrador de textura

void main() {
    // Amostra a cor da textura usando as coordenadas passadas do vertex shader
    outColor = texture(textureSampler, fragTexCoord);
}



