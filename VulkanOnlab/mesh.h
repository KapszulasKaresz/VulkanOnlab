#pragma once
#include <vector>
#include "vertex.h"
#include <stdexcept>
#include <unordered_map>


struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	Mesh() {}
	void load(const char* filename);
};