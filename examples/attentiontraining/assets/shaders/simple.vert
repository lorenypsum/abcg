#version 300 es

layout(location = 0) in vec2 aPos; // Posição dos vértices
layout(location = 1) in vec3 aColor; // Cor dos vértices

out vec3 vertexColor; // Variável para passar a cor para o fragment shader

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0); // Define a posição do vértice
    vertexColor = aColor; // Passa a cor do vértice
}


