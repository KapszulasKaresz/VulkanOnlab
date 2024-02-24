#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include "light.h"
#define MAX_LIGHTS 20


struct UniformBufferObject {
	glm::mat4 view;
	glm::mat4 proj;
	alignas(16)glm::vec3 wEye;
	Light lights[MAX_LIGHTS];
	uint32_t numLights;
};