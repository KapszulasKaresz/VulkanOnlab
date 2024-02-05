#pragma once
#include "transformation.h"

struct Scale : Transformation {
	glm::vec3 scale;
	glm::mat4 getMatrix();
};