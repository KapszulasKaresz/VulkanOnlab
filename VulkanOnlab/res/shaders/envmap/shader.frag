#version 450

struct Light {
    vec4 pos;
    vec3 La;
    vec3 Le;
};


layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
    vec3 wEye;
    Light lights[20];
    int numLights;
} ubo;

layout(set = 2, binding = 0) uniform ObjectUniformBufferObject {
    mat4 model;
    mat4 modelInverse;
} oubo;

layout(set = 1, binding = 1) uniform samplerCube envMap;

layout(location = 0) in vec2 inTexCoord;
layout(location = 1) in vec4 inRayDir;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(envMap, inRayDir.xyz);
}