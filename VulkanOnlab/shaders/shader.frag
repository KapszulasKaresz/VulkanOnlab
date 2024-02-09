#version 450


layout(binding = 1) uniform sampler2D texSampler;
layout( binding = 2) uniform Material {
    float shininess;
    vec3 ks;
    vec3 kd;
    vec3 ka;
} mat;

layout(location = 0) in vec3 wNormal;
layout(location = 1) in vec3 wView;
layout(location = 3) in vec4 wPos;
layout(location = 4) in vec2 texCoord;

struct Light {
    vec4 pos;
    vec3 La;
    vec3 Le;
};

layout(location = 0) out vec4 outColor;

void main() {
    Light l;
    l.pos =  vec4(10.0, 10.0,10.0, 1.0);
    l.La = vec3(0.2, 0.2, 0.2);
    l.Le = vec3(1);

    vec3 wLight = l.pos.xyz * wPos.w - wPos.xyz * l.pos.w;

    vec3 N = normalize(wNormal);
	vec3 V = normalize(wView); 

    if (dot(N, V) < 0) N = -N;

    vec3 ka = texture(texSampler, texCoord).xyz;
    vec3 kd = texture(texSampler, texCoord).xyz;

    vec3 radiance = vec3(0, 0, 0);

    vec3 L = normalize(wLight);
	vec3 H = normalize(L + V);
    float cost = max(dot(N,L), 0), cosd = max(dot(N,H), 0);

    radiance += ka * l.La + (kd * cost + mat.ks * pow(cosd, mat.shininess)) * l.Le;

    outColor = vec4(radiance, 1.0);
}