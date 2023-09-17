#version 450

layout(binding = 0) uniform modelMatrix {
    mat4 modelLocation;
} model;

layout(binding = 1) uniform ProjectionMatrix {
    mat4 ortho;
} projMatrix;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec3 fragColor;

void main() {

    gl_Position = projMatrix.ortho * model.modelLocation * vec4(inPosition, 0.5f, 1.0f);
    fragColor = inColor;
}