#version 460
#extension GL_EXT_ray_query : enable

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

layout(set = 0, binding = 1) uniform accelerationStructureEXT topLevelAS;


layout(set = 2, binding = 0) uniform ObjectUniformBufferObject {
    mat4 model;
    mat4 modelInverse;
} oubo;

layout(set = 1, binding = 0) uniform Material {
    float shininess;
    vec3 ks;
    vec3 kd;
    vec3 ka;
} mat;

layout(location = 0) in vec3 wNormal;
layout(location = 1) in vec3 wView;
layout(location = 3) in vec4 wPos;
layout(location = 4) in vec2 texCoord;

layout(location = 0) out vec4 outColor;

bool intersects_light(vec3 direction, vec3 pos)
{
	const float tmin = 0.01, tmax = 1000;

	rayQueryEXT query;

	rayQueryInitializeEXT(query, topLevelAS, gl_RayFlagsTerminateOnFirstHitEXT, 0xFF, pos, tmin, direction.xyz, tmax);

	rayQueryProceedEXT(query);
	if (rayQueryGetIntersectionTypeEXT(query, true) != gl_RayQueryCommittedIntersectionNoneEXT)
	{
		return true;
	}
	return false;
}

void main() {
    vec3 N = normalize(wNormal);
	vec3 V = normalize(wView); 

    if (dot(N, V) < 0) N = -N;

    vec3 ka = mat.ka;
    vec3 kd = mat.kd;

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

        
        if(!intersects_light(L, wPos.xyz)) {
            radiance += (ka * ubo.lights[i].La + (kd * cost + mat.ks * pow(cosd, mat.shininess)) * ubo.lights[i].Le) / (dist* dist);
        }
    }
    outColor = vec4(radiance, 1.0);
}