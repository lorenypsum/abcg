// Vertex Shader
#version 300 es

layout(location = 0) in vec3 inPosition;

out vec3 fragTexCoord;

uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
    fragTexCoord = normalize(inPosition); // Pass normalized coordinates to the fragment shader

    // Ensure viewMatrix does not translate the cubemap, keeping it sharp
    mat4 viewMatrixNoTranslation = mat4(
        vec4(viewMatrix[0].xyz, 0.0),
        vec4(viewMatrix[1].xyz, 0.0),
        vec4(viewMatrix[2].xyz, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );

    vec4 P = projMatrix * viewMatrixNoTranslation * vec4(inPosition, 1.0);
    gl_Position = P.xyww;
}