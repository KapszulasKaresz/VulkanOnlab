#include "vulkan/mesh/mesh.h"
#include "vulkan/application.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void Mesh::load(const char* filename)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename)) {
		throw std::runtime_error(warn + err);
	}

	std::unordered_map<Vertex, uint32_t> uniqueVertices{};

	for (const auto& shape : shapes) {
		for (int i = 0; i < shape.mesh.indices.size(); i++) {
			Vertex vertex{};
			auto index = shape.mesh.indices[i];

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.normal = {
				attrib.normals[3 * index.normal_index + 0],
				attrib.normals[3 * index.normal_index + 1],
				attrib.normals[3 * index.normal_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			auto index1 = shape.mesh.indices[i];
			auto index2 = shape.mesh.indices[i];

			if (i % 3 == 0) {
				index = shape.mesh.indices[i + 0];
				index1 = shape.mesh.indices[i + 1];
				index2 = shape.mesh.indices[i + 2];
			}
			else if (i % 3 == 1) {
				index = shape.mesh.indices[i - 1];
				index1 = shape.mesh.indices[i + 0];
				index2 = shape.mesh.indices[i + 1];
			}
			else if (i % 3 == 2) {
				index = shape.mesh.indices[i - 2];
				index1 = shape.mesh.indices[i - 1];
				index2 = shape.mesh.indices[i + 0];
			}

			glm::vec3 edge1 = glm::vec3(attrib.vertices[3 * index1.vertex_index + 0],
				attrib.vertices[3 * index1.vertex_index + 1],
				attrib.vertices[3 * index1.vertex_index + 2]) - glm::vec3(attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]);

			glm::vec3 edge2 = glm::vec3(attrib.vertices[3 * index2.vertex_index + 0],
				attrib.vertices[3 * index2.vertex_index + 1],
				attrib.vertices[3 * index2.vertex_index + 2]) - glm::vec3(attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]);

			glm::vec2 deltaUV1 = glm::vec2(attrib.texcoords[2 * index1.texcoord_index + 0],
				attrib.texcoords[2 * index1.texcoord_index + 1]) - glm::vec2(attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]);

			glm::vec2 deltaUV2 = glm::vec2(attrib.texcoords[2 * index2.texcoord_index + 0],
				attrib.texcoords[2 * index2.texcoord_index + 1]) - glm::vec2(attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]);

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			vertex.tangent = {
				f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
				f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
				f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
			};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);
		}
	}

	if (vertexBuffer != VK_NULL_HANDLE) {
		freeVertexBuffer();
	}

	createVertexBuffer();
	createIndexBuffer();
}

Mesh::~Mesh()
{
	freeVertexBuffer();
}

void Mesh::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Application::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(
		Application::device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(Application::device, stagingBufferMemory);

	Application::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer, vertexBufferMemory);

	Application::copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(Application::device, stagingBuffer, nullptr);
	vkFreeMemory(Application::device, stagingBufferMemory, nullptr);
}

void Mesh::createIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Application::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(Application::device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(Application::device, stagingBufferMemory);

	Application::createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	Application::copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(Application::device, stagingBuffer, nullptr);
	vkFreeMemory(Application::device, stagingBufferMemory, nullptr);
}

void Mesh::freeVertexBuffer()
{
	vkDestroyBuffer(Application::device, indexBuffer, nullptr);
	vkFreeMemory(Application::device, indexBufferMemory, nullptr);

	vkDestroyBuffer(Application::device, vertexBuffer, nullptr);
	vkFreeMemory(Application::device, vertexBufferMemory, nullptr);
}
