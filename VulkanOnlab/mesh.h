#pragma once
#include <vector>
#include "vertex.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>


struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	void load(const char* filename);
};