#pragma once
#include "vulkan/mesh/mesh.h"
#include <glm/glm.hpp>
#include "vulkan/transform/transformation.h"
#include "vulkan/camera.h"
#include "vulkan/material/material.h"
#include "objectuniformbufferobject.h"
#include "vulkan/light/light.h"
#include <vector>
#include <optional>

class Object {
public:
	int id;
	Mesh* mesh = nullptr;
	Material* material = nullptr;

	Object()
		:
		id(rollingId++) {}

	std::vector<Transformation*> transformations;

	glm::mat4 getModelMatrix();
	void create(const char* meshFilename);

	void updateUniformBuffer(uint32_t currentImage, Camera& camera, std::vector<Light*>& lights);

	void cleanup();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t currentFrame);

	bool operator==(Object& other) { return id == other.id; }

	std::vector<VkDescriptorSet> descriptorSets;

	static VkDescriptorSetLayout descriptorSetLayout;

	~Object();
private:
	static int rollingId;

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


	static void createDescriptorSetLayout();

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	void createUniformBuffers();

	void createDescriptorSets();
	void createMaterial();
};