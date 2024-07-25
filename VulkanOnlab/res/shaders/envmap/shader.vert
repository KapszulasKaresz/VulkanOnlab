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

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 inTangent;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec4 outRayDir;

mat4 BuildTranslation(vec3 delta)
{
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(delta, 1.0));
}

void main() {
    outTexCoord = texCoord;

    mat4 rayDirMatrix = BuildTranslation(ubo.wEye);
    mat4 viewProj =  ubo.proj *ubo.view;
    rayDirMatrix = viewProj *rayDirMatrix;
    rayDirMatrix = inverse(rayDirMatrix);

    gl_Position = vec4(inPos.xy,  0.9999999, 1.0);
   
    outRayDir =  rayDirMatrix * vec4(inPos, 1.0);
}