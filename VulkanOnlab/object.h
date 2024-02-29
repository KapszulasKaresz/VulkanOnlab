#pragma once
#include "mesh.h"
#include "texture.h"
#include <glm/glm.hpp>
#include "transformation.h"
#include "camera.h"
#include "material.h"
#include "objectuniformbufferobject.h"
#include "light.h"
#include <vector>
#include <optional>

class Object {
public:	
	int id;
	Mesh* mesh = nullptr;
	Material* material = nullptr;

	Object(VkDevice& device, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue
		, VkExtent2D& swapChainExtent, VkRenderPass& renderPass, VkSurfaceKHR& surface,
		VkCommandPool& commandPool, VkDescriptorPool& descriptorPool, 
		VkDescriptorSetLayout& globalDescriptorSetLayout, std::vector<VkDescriptorSet>& globalDescriptorSets)
		: device(device), physicalDevice(physicalDevice), graphicsQueue(graphicsQueue)
		, swapChainExtent(swapChainExtent), renderPass(renderPass), surface(surface), 
		commandPool(commandPool), descriptorPool(descriptorPool),
		globalDescriptorSetLayout(globalDescriptorSetLayout), globalDescriptorSets(globalDescriptorSets),
		id(rollingId++){}
	
	std::vector<Transformation*> transformations;

	glm::mat4 getModelMatrix();
	void create(const char* meshFilename);

	void recreatePipeline();
	void updateUniformBuffer(uint32_t currentImage, Camera& camera, std::vector<Light*>& lights);

	void cleanup();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t currentFrame);

	bool operator==(Object& other) { return id == other.id; }

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	std::vector<VkDescriptorSet> descriptorSets;
	~Object();
private:
	static int rollingId;
	VkDevice& device;
	VkPhysicalDevice& physicalDevice;
	VkQueue& graphicsQueue;
	VkExtent2D& swapChainExtent;
	VkRenderPass& renderPass;
	VkSurfaceKHR& surface;
	VkCommandPool& commandPool;
	VkDescriptorPool& descriptorPool;

	VkDescriptorSetLayout& globalDescriptorSetLayout;
	std::vector<VkDescriptorSet>& globalDescriptorSets;

	VkDescriptorSetLayout descriptorSetLayout;
	

	void createGraphicsPipeline();
	static std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);


	VkDeviceMemory vertexBufferMemory;
	void createVertexBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	VkDeviceMemory indexBufferMemory;
	void createIndexBuffer();

	void createDescriptorSetLayout();

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	void createUniformBuffers();

	void createDescriptorSets();
	void createMaterial(const char* filename);
};