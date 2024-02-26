#pragma once
#include "materialuniformbufferobject.h"
#include "texture.h"

struct Material {
	Texture* albedoTexture;
	SharedGraphicsInfo graphInfo;

	MaterialUniformBufferObject material;

	Material(VkDevice& device, VkDescriptorPool& descriptorPool, VkPhysicalDevice& physicalDevice,const char* texturePath, SharedGraphicsInfo graphInfo);

	void createDescriptorSetLayout();
	void createDescriptorSets();
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;


	void createUniformBuffers();
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	void swapAlbedoTexture(Texture* newTexture);
	void swapAlbedoTexture(const char* filename);

	~Material();
private:
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkDescriptorPool& descriptorPool;
	VkDevice& device;
	VkPhysicalDevice& physicalDevice;
};