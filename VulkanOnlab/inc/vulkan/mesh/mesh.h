#pragma once
#include <vector>
#include "vulkan/mesh/vertex.h"
#include <stdexcept>
#include <unordered_map>


struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	Mesh() {}
	void load(const char* filename);

	VkBuffer vertexBuffer = VK_NULL_HANDLE;
	VkBuffer indexBuffer = VK_NULL_HANDLE;

	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;

	~Mesh();
private:
	void createVertexBuffer();
	void createIndexBuffer();
	void freeVertexBuffer();
};