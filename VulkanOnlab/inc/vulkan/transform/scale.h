#pragma once
#include "vulkan/transform/transformation.h"

struct Scale : public Transformation {
	glm::vec3 scale;

	Scale(glm::vec3 scale) : scale(scale) {}

	std::string getName() { return "Scale"; }

	glm::mat4 getMatrix();
};