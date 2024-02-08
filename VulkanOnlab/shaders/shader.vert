#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 modelInverse;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inColor;
layout(location = 3) in vec2 texCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main() {
    fragColor = (ubo.modelInverse * vec4(inNormal, 0.0)).xyz;
    fragTexCoord = texCoord;
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPos , 1.0);
}