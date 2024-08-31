#pragma once
#include "vulkan/transform/transformation.h"

struct Translation : public Transformation {
	glm::vec3 translation;

	Translation(glm::vec3 translation) : translation(translation) {}

	std::string getName() { return "Translation"; }

	glm::mat4 getMatrix();
};