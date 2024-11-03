#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
  FragColor = vec4(vertexColor, 1.0); // Define a cor de saída com alpha 1.0
}

