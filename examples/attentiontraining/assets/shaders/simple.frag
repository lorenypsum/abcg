#version 330 core

in vec3 vertexColor; // Recebe a cor do vertex shader
out vec4 FragColor; // Cor final do fragmento

void main() {
    FragColor = vec4(vertexColor, 1.0); // Define a cor do fragmento
}

