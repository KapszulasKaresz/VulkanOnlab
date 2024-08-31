#pragma once
#include "vulkan/mesh/mesh.h"
#include <glm/glm.hpp>
#include "vulkan/camera.h"
#include "vulkan/material/material.h"
#include "objectuniformbufferobject.h"
#include "vulkan/light/light.h"
#include "vulkan/framework/accelerationstructure.h"
#include "vulkan/object/node.h"
#include <vector>
#include <optional>

namespace tinygltf {
	struct Primitive;
	class Model;
}

class Object : public RenderNode {
public:
	
	Mesh* mesh = nullptr;

	Object()
		:
		RenderNode() {}


	void create(const char* meshFilename);
	void create(tinygltf::Primitive* primitive, tinygltf::Model* gltfModel);

	Material* getMaterial() { return material; }
	void swapMaterial(Material* material);

	void updateUniformBuffer(uint32_t currentImage, Camera& camera, std::vector<Light*>& lights);

	void cleanup();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t currentFrame);

	bool operator==(Object& other) { return id == other.id; }

	std::vector<VkDescriptorSet> descriptorSets;

	static VkDescriptorSetLayout descriptorSetLayout;

	static void createDescriptorSetLayout();

	void checkTransformationUpdate();

	bool getShadowCast();

	void toggleShadowCast();

	VkAccelerationStructureInstanceKHR instance;

	bool accelerationStructureDirty = false;
	bool hasAccelerationStructure = false;

	~Object();
private:
	bool castShadow = true;
	
	enum ShadowBits : uint32_t {
		CAST_SHADOW = 0xFF,
		NO_CAST_SHADOW = 0xFE
	};


	Material* material = nullptr;

	VkTransformMatrixKHR convertToVkTransformMatrixKHR(const glm::mat4& mat);

	void createBottomLevelAccelerationStructure();
	void updateBottomLevelAccelerationStructureTransform();

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkBuffer transformMatrixBuffer = VK_NULL_HANDLE;
	VkDeviceMemory transformMatrixBufferMemory;

	AccelerationStructure* bottomLevelAS = nullptr;

	VkDeviceMemory instanceBufferMemory;

	glm::mat4 oldTransform = glm::mat4(1.0);

	void createUniformBuffers();

	void createDescriptorSets();
	void createMaterial();
};