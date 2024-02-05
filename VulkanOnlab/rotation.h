#pragma once
#include <glm/glm.hpp>
#include "transformation.h"

struct Rotation : Transformation {
	glm::vec3 rotationAxis;
	float amount;

	glm::mat4 getMatrix();
};