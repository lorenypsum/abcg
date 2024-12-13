// Fragment Shader
#version 300 es

precision mediump float;

in vec3 fragTexCoord;

out vec4 outColor;

uniform samplerCube skyTex;

void main() {
    // Directly sample the cubemap texture with normalized coordinates
    outColor = texture(skyTex, fragTexCoord);
}