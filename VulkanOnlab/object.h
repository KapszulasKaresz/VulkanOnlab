#pragma once
#include "mesh.h"
#include "texture.h"
#include <glm/glm.hpp>
#include "transformation.h"
#include "camera.h"
#include <vector>
#include <optional>

class Object {
public:	
	Mesh* mesh;
	Texture* texture;

	Object(VkDevice& device, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue
		, VkExtent2D& swapChainExtent, VkRenderPass& renderPass, VkSurfaceKHR& surface, VkCommandPool& commandPool)
		: device(device), physicalDevice(physicalDevice), graphicsQueue(graphicsQueue)
		, swapChainExtent(swapChainExtent), renderPass(renderPass), surface(surface), commandPool(commandPool){}
	
	std::vector<Transformation*> transformations;

	glm::mat4 getModelMatrix();

	void create();
	void updateUniformBuffer(uint32_t currentImage, Camera& camera);

	void cleanup();
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t currentFrame);

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkBuffer vertexBuffer;
	VkBuffer indexBuffer;
	std::vector<VkDescriptorSet> descriptorSets;
private:
	VkDevice& device;
	VkPhysicalDevice& physicalDevice;
	VkQueue& graphicsQueue;
	VkExtent2D& swapChainExtent;
	VkRenderPass& renderPass;
	VkSurfaceKHR& surface;
	VkCommandPool& commandPool;

	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;

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

	void createDescriptorPool();
	void createDescriptorSets();
	void createTexture();
};