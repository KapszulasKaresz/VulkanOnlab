#pragma once
#include <glm/glm.hpp>
#include "vulkan/transform/transformation.h"

struct MatrixTransform : public Transformation {

	MatrixTransform(glm::mat4 transformMatrix) : transformMatrix(transformMatrix) {}

	std::string getName() { return "MatrixTransform"; }

	glm::mat4 transformMatrix;

	glm::mat4 getMatrix();
};