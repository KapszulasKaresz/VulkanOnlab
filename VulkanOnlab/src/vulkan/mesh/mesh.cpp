#include "vulkan/mesh/mesh.h"
#include "vulkan/application.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <tinygltf/tiny_gltf.h>

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

void Mesh::load(tinygltf::Primitive* primitive, tinygltf::Model* gltfModel)
{
	std::vector<float> positions, normals, texcoords, tangents;

	// Get positions
	if (primitive->attributes.find("POSITION") != primitive->attributes.end()) {
		const auto& accessor = gltfModel->accessors[primitive->attributes.at("POSITION")];
		getAttributeData(*gltfModel, accessor, positions);
	}

	// Get normals
	if (primitive->attributes.find("NORMAL") != primitive->attributes.end()) {
		const auto& accessor = gltfModel->accessors[primitive->attributes.at("NORMAL")];
		getAttributeData(*gltfModel, accessor, normals);
	}

	// Get texture coordinates
	if (primitive->attributes.find("TEXCOORD_0") != primitive->attributes.end()) {
		const auto& accessor = gltfModel->accessors[primitive->attributes.at("TEXCOORD_0")];
		getAttributeData(*gltfModel, accessor, texcoords);
	}

	// Get tangents
	if (primitive->attributes.find("TANGENT") != primitive->attributes.end()) {
		const auto& accessor = gltfModel->accessors[primitive->attributes.at("TANGENT")];
		getAttributeData(*gltfModel, accessor, tangents);
	}

	// Get indices
	std::vector<uint32_t> localIndices;

	if (primitive->indices >= 0) {
		const auto& indexAccessor = gltfModel->accessors[primitive->indices];
		const auto& indexBufferView = gltfModel->bufferViews[indexAccessor.bufferView];
		const auto& indexBuffer = gltfModel->buffers[indexBufferView.buffer];

		const unsigned char* dataPtr = indexBuffer.data.data() + indexBufferView.byteOffset + indexAccessor.byteOffset;

		localIndices.resize(indexAccessor.count);

		// Handle different index component types
		switch (indexAccessor.componentType) {
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
			for (size_t i = 0; i < indexAccessor.count; ++i) {
				localIndices[i] = *(reinterpret_cast<const uint8_t*>(dataPtr) + i);
			}
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
			for (size_t i = 0; i < indexAccessor.count; ++i) {
				localIndices[i] = *(reinterpret_cast<const uint16_t*>(dataPtr) + i);
			}
			break;
		case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
			for (size_t i = 0; i < indexAccessor.count; ++i) {
				localIndices[i] = *(reinterpret_cast<const uint32_t*>(dataPtr) + i);
			}
			break;
		default:
			std::cerr << "Error: Unsupported index component type" << std::endl;
			return;
		}
	}

	size_t vertexCount = positions.size() / 3;

	for (size_t i = 0; i < vertexCount; ++i) {
		Vertex vertex{};

		vertex.pos = {
			positions[3 * i + 0],
			positions[3 * i + 1],
			positions[3 * i + 2]
		};

		if (!normals.empty()) {
			vertex.normal = {
			normals[3 * i + 0],
			normals[3 * i + 1],
			normals[3 * i + 2]
			};
		}

		if (!texcoords.empty()) {
			vertex.texCoord = {
				texcoords[2 * i + 0],
				1.0f - texcoords[2 * i + 1]
			};
		}

		if (!tangents.empty()) {
			vertex.tangent = {
				tangents[i * 4 + 0] * tangents[i * 4 + 3],
				tangents[i * 4 + 1] * tangents[i * 4 + 3],
				tangents[i * 4 + 2] * tangents[i * 4 + 3]
			};
		}

		vertices.push_back(vertex);
	}

	// Convert the primitive to GL_TRIANGLES mode
	if (primitive->mode == TINYGLTF_MODE_TRIANGLES) {
		indices.insert(indices.end(), localIndices.begin(), localIndices.end());
	}
	else if (primitive->mode == TINYGLTF_MODE_TRIANGLE_STRIP) {
		for (size_t i = 0; i < localIndices.size() - 2; ++i) {
			if (i % 2 == 0) {
				indices.push_back(localIndices[i]);
				indices.push_back(localIndices[i + 1]);
				indices.push_back(localIndices[i + 2]);
			}
			else {
				indices.push_back(localIndices[i]);
				indices.push_back(localIndices[i + 2]);
				indices.push_back(localIndices[i + 1]);
			}
		}
	}
	else if (primitive->mode == TINYGLTF_MODE_TRIANGLE_FAN) {
		for (size_t i = 1; i < localIndices.size() - 1; ++i) {
			indices.push_back(localIndices[0]);
			indices.push_back(localIndices[i]);
			indices.push_back(localIndices[i + 1]);
		}
	}
	else {
		std::cerr << "Error: Unsupported primitive mode" << std::endl;
		return;
	}

	if (tangents.empty()) {
		int index;
		int index1;
		int index2;
		for (int i = 0; i < indices.size(); i++) {
			if (i % 3 == 0) {
				index = indices[i + 0];
				index1 = indices[i + 1];
				index2 = indices[i + 2];
			}
			else if (i % 3 == 1) {
				index = indices[i - 1];
				index1 = indices[i + 0];
				index2 = indices[i + 1];
			}
			else if (i % 3 == 2) {
				index = indices[i - 2];
				index1 = indices[i - 1];
				index2 = indices[i + 0];
			}

			glm::vec3 edge1 = vertices[index1].pos - vertices[index].pos;

			glm::vec3 edge2 = vertices[index2].pos - vertices[index].pos;

			glm::vec2 deltaUV1 = vertices[index1].texCoord - vertices[index].texCoord;

			glm::vec2 deltaUV2 = vertices[index2].texCoord - vertices[index].texCoord;

			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			vertices[indices[i]].tangent = {
				f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
				f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
				f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
			};
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

void Mesh::getAttributeData(const tinygltf::Model& model, const tinygltf::Accessor& accessor, std::vector<float>& out)
{
	const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
	const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

	const unsigned char* dataPtr = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
	size_t componentCount = tinygltf::GetNumComponentsInType(accessor.type);
	size_t count = accessor.count;
	size_t stride = accessor.ByteStride(bufferView) ? accessor.ByteStride(bufferView) : componentCount * sizeof(float);

	out.resize(count * componentCount);
	for (size_t i = 0; i < count; ++i) {
		const float* sourcePtr = reinterpret_cast<const float*>(dataPtr + i * stride);
		std::copy(sourcePtr, sourcePtr + componentCount, out.begin() + i * componentCount);
	}
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

	Application::createBuffer(bufferSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
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

	Application::createBuffer(bufferSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

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
