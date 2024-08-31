#pragma once
#include <glm/glm.hpp>
#include "vulkan/transform/transformation.h"

struct MatrixTransform : Transformation {

	MatrixTransform(glm::mat4 transformMatrix) : transformMatrix(transformMatrix) {}

	glm::mat4 transformMatrix;

	glm::mat4 getMatrix();
};