#pragma once
#include <glm/glm.hpp>
#include "vulkan/transform/transformation.h"

struct Rotation : public Transformation {
	glm::vec3 rotationAxis;
	float amount;

	Rotation(glm::vec3 rotationAxis, float amount) : rotationAxis(rotationAxis), amount(amount) {}

	std::string getName() { return "Rotation"; }

	glm::mat4 getMatrix();
};