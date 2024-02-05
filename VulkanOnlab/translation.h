#pragma once
#include "transformation.h"

struct Translation : Transformation {
	glm::vec3 translation;

	Translation(glm::vec3 translation) : translation(translation) {}

	glm::mat4 getMatrix();
};