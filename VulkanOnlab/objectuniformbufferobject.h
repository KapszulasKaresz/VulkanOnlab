#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

struct ObjectUniformBufferObject {
	glm::mat4 model;
	glm::mat4 modelInverse;
};