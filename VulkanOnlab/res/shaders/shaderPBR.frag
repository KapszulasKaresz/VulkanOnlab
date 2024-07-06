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
    float maxMipLevel;  
} mat;

layout(set = 1, binding = 1) uniform sampler2D brdfLUT;
layout(set = 1, binding = 2) uniform samplerCube envMap;
layout(set = 1, binding = 3) uniform samplerCube irradianceMap;

layout(location = 0) in vec3 wNormal;
layout(location = 1) in vec3 wView;
layout(location = 3) in vec4 wPos;
layout(location = 4) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

#define PI 3.1415926

float phongDiffuse()
{
    return (1.0 / PI);
}

vec3 fresnel(vec3 F0, float product) {
    return mix(F0, vec3(1.0), pow(1.01 - product, 5.0));
}

float D_GGX(float roughness, float NdH) {
    float m2 = roughness * roughness * roughness * roughness;
    float d = (NdH * m2 - NdH) * NdH + 1.0;
    return m2 / (PI * d * d);
}

float G_schlick(float roughness, float NdV, float NdL) {
    float k = roughness * roughness * 0.5;
    float V = NdV * (1.0 - k) + k;
    float L = NdL * (1.0 - k) + k;
    return 0.25 / (V * L);
}

vec3 blinnSpecular(in float NdH, in vec3 specular, in float roughness)
{
    float k = 1.999 / (roughness * roughness);
    
    return min(1.0, 3.0 * 0.0398 * k) * pow(NdH, min(10000.0, k)) * specular;
}

vec3 cooktorranceSpecular(float NdL, float NdV, float NdH, vec3 specular, float roughness) {
    float D = D_GGX(roughness, NdH);
    float G = G_schlick(roughness, NdV, NdL);

    return specular * G * D;
}

void main() {
    vec3 N = normalize(wNormal);
	vec3 V = normalize(wView); 
    vec3 albedo = mat.albedo;
    float metallic = mat.metallic;
    float roughness = mat.roughness;
    roughness = clamp(roughness, 0.001, 1.0);
    float NdV = max(0.001, dot(N, V));
    vec3 specular = mix(vec3(0.04), albedo, metallic);

    if (dot(N, V) < 0) { 
        N = -N;
    }
    
    vec3 radiance = vec3(0);
    vec3 reflectedLight = vec3(0);
    vec3 diffuseLight = vec3(0);

    for(int i = 0; i < ubo.numLights; i++) {
        vec3 wLight = ubo.lights[i].pos.xyz * wPos.w - wPos.xyz * ubo.lights[i].pos.w;
        vec3 L = normalize(wLight);
	    vec3 H = normalize(L + V);
        
        float NdL = max(0.0, dot(N, L));
        float NdH = max(0.001, dot(N, H));
        float HdV = max(0.001, dot(H, V));
        float LdV = max(0.001, dot(L, V));

        vec3 specfresnel = fresnel(specular, HdV);
        vec3 specref = cooktorranceSpecular(NdL, NdV, NdH, specfresnel, roughness);

        specref *= vec3(NdL);

        vec3 diffref = (vec3(1.0) - specfresnel) * phongDiffuse() * NdL;

        float dist = length(wLight);
        if(ubo.lights[i].pos.w < 0.5) {
            dist = 1.0f;
        }

        vec3 lightColor = ubo.lights[i].Le * 20 / (dist * dist);
        reflectedLight += specref * lightColor;
        diffuseLight += diffref * lightColor;
    }

    vec3 envdiff = textureLod(irradianceMap, N, 0.0f).xyz;

    vec3 refl = reflect(-V, N);
    vec3 envspec = textureLod(envMap, refl, max(roughness * mat.maxMipLevel, textureQueryLod(envMap, refl).y)).xyz;

    vec2 brdf = texture(brdfLUT, vec2(roughness, 1.0 - NdV)).xy;
    vec3 iblspec = min(vec3(0.99), fresnel(specular, NdV) * brdf.x + brdf.y);
    reflectedLight += iblspec * envspec;
    diffuseLight += envdiff * (1.0 / (PI));
    
    radiance = diffuseLight * mix(albedo, vec3(0.0), metallic) + reflectedLight;


    outColor = vec4(radiance, 1.0);
}