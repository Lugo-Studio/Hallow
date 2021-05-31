#version 450

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 diffuseColor;

layout(push_constant) uniform Push {
    mat2 transform;
    // vec3 color;
} push;


void main() {
    diffuseColor = vec4(fragColor, 1.0);
    // diffuseColor = vec4(push.color, 1.0);
}
