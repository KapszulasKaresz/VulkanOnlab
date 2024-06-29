#pragma once
#include <glm/glm.hpp>
#include "vulkan/transform/transformation.h"

struct Rotation : Transformation {
	glm::vec3 rotationAxis;
	float amount;

	Rotation(glm::vec3 rotationAxis, float amount) : rotationAxis(rotationAxis), amount(amount) {}

	glm::mat4 getMatrix();
};