#pragma once
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

struct Light {
	alignas(16)glm::vec4 pos;
	alignas(16)glm::vec3 La;
	alignas(16)glm::vec3 Le;

	Light() : pos(glm::vec4(1)), La(glm::vec3(1)), Le(glm::vec3(1)) {}
	Light(glm::vec4 pos, glm::vec3 La, glm::vec3 Le) : pos(pos), La(La), Le(Le) {}
};