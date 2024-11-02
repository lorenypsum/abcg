#version 450 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 fragTexCoord;

uniform vec2 translation;
uniform float scale;
uniform float rotation;

void main() {
    // Aplicar rotação
    mat2 rotationMatrix = mat2(cos(rotation), -sin(rotation),
                               sin(rotation), cos(rotation));
    
    vec2 transformedPosition = (rotationMatrix * inPosition * scale) + translation;
    
    // Define a posição final do vértice
    gl_Position = vec4(transformedPosition, 0.0, 1.0);
    
    fragTexCoord = inTexCoord;
}



