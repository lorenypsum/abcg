#version 300 es

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 TexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
    TexCoord = inTexCoord;
    gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}
