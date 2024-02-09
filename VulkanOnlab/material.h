#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

struct Material {
	float shininess;
	alignas(16)glm::vec3 ks;
	alignas(16)glm::vec3 kd;
	alignas(16)glm::vec3 ka;
};