#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 modelInverse;
	glm::mat4 view;
	glm::mat4 proj;
	glm::vec3 wEye;
};