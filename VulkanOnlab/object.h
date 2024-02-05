#pragma once
#include "mesh.h"
#include "texture.h"
#include <glm/glm.hpp>
#include "transformation.h"
#include <vector>

struct Object {
	Mesh* mesh;
	Texture* texture;
	
	std::vector<Transformation*> transformations;

	glm::mat4 getModelMatrix();

};