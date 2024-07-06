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


layout(location = 0) in vec3 wNormal;
layout(location = 1) in vec3 wView;
layout(location = 3) in vec4 wPos;
layout(location = 4) in vec2 texCoord;
layout(location = 5) in mat3 TBN;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform Material {
	float shininess;
	vec3 ks;
	vec3 kd;
	vec3 ka;
} mat;

layout(set = 1, binding = 1) uniform sampler2D texSampler1;

void main() {
	vec3 N = texture(texSampler1, texCoord).xyz;
	N = N * 2.0 - vec3(1.0);
	N = normalize(TBN * N);
	vec3 V = normalize(wView); 
	vec3 kd = mat.kd;
	vec3 ka = mat.ka;
	vec3 ks = mat.ks;
	float shininess = mat.shininess;

	vec3 radiance = vec3(0, 0, 0);

	for(int i = 0; i < ubo.numLights; i++){
		vec3 wLight = ubo.lights[i].pos.xyz * wPos.w - wPos.xyz * ubo.lights[i].pos.w;
		vec3 L = normalize(wLight);
		vec3 H = normalize(L + V);
		float cost = max(dot(N,L), 0), cosd = max(dot(N,H), 0);
		float dist = length(wLight);
		if(ubo.lights[i].pos.w < 0.5) {
			dist = 1.0f;
		}

		radiance += (ka * ubo.lights[i].La + (kd * cost + ks * pow(cosd, shininess)) * ubo.lights[i].Le) / (dist* dist);
	}
	outColor = vec4(radiance, 1.0);
}
