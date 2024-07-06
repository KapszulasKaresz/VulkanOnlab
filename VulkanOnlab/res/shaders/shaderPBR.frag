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

layout(set = 1, binding = 0) uniform Material {
    vec3 albedo;
    float metallic;
    float roughness;
} mat;

layout(location = 0) in vec3 wNormal;
layout(location = 1) in vec3 wView;
layout(location = 3) in vec4 wPos;
layout(location = 4) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 N = normalize(wNormal);
	vec3 V = normalize(wView); 

    if (dot(N, V) < 0) N = -N;

    outColor = vec4(mat.albedo, 1.0);
}