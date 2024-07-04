#pragma once
#include "materialuniformbufferobject.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <string>

class Texture;

struct Material {
	std::vector<Texture*> textures;

	MaterialUniformBufferObject material;

	Material();

	void createDescriptorSetLayout();
	void createDescriptorSets();
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;

	void recreatePipeline(const char* fragmentFileName);

	void recreateDescriptors();
	void setTexture(std::vector<Texture*> textures);

	void updateUniformBuffer(uint32_t currentImage);

	void createUniformBuffers();
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	~Material();
private:
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void createGraphicsPipeline(const char* fragmentFileName);
	static std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);
};