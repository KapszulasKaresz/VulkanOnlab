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

	Object()
		:
		id(rollingId++) {}

	std::vector<Transformation*> transformations;

	glm::mat4 getModelMatrix();
	void create(const char* meshFilename);

	Material* getMaterial() { return material; }
	void swapMaterial(Material* material);

	void updateUniformBuffer(uint32_t currentImage, Camera& camera, std::vector<Light*>& lights);

	void cleanup();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t currentFrame);

	bool operator==(Object& other) { return id == other.id; }

	std::vector<VkDescriptorSet> descriptorSets;

	static VkDescriptorSetLayout descriptorSetLayout;

	static void createDescriptorSetLayout();

	~Object();
private:
	static int rollingId;
	
	Material* material = nullptr;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	void createUniformBuffers();

	void createDescriptorSets();
	void createMaterial();
};