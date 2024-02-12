#version 450

struct Light {
    vec4 pos;
    vec3 La;
    vec3 Le;
};

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 modelInverse;
    mat4 view;
    mat4 proj;
    vec3 wEye;
    Light lights[20];
    int numLights;
} ubo;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 texCoord;

layout(location = 0) out vec3 wNormal;
layout(location = 1) out vec3 wView;
layout(location = 3) out vec4 wPos;
layout(location = 4) out vec2 outTexCoord;


void main() {
    wPos =  ubo.model * vec4(inPos , 1.0);
    wNormal = (ubo.modelInverse * vec4(inNormal, 0.0)).xyz;
    wView = (ubo.wEye * wPos.w).xyz - wPos.xyz;
    outTexCoord = texCoord;
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPos , 1.0);
}