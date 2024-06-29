#pragma once
#include "vulkan/transform/transformation.h"

struct Scale : Transformation {
	glm::vec3 scale;

	Scale(glm::vec3 scale) : scale(scale) {}

	glm::mat4 getMatrix();
};