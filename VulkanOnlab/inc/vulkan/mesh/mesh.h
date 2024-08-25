#pragma once
#include <vector>
#include "vulkan/mesh/vertex.h"
#include <stdexcept>
#include <unordered_map>

namespace tinygltf {
	struct Primitive;
	class Model;
	struct Accessor;
}

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	Mesh() {}
	void load(const char* filename);
	void load(tinygltf::Primitive* primitve, tinygltf::Model* gltfModel);

	VkBuffer vertexBuffer = VK_NULL_HANDLE;
	VkBuffer indexBuffer = VK_NULL_HANDLE;

	VkDeviceMemory vertexBufferMemory;
	VkDeviceMemory indexBufferMemory;

	~Mesh();
private:
	void getAttributeData(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<float>& out);

	void createVertexBuffer();
	void createIndexBuffer();
	void freeVertexBuffer();
};