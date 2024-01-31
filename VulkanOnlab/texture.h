#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include "sharedgraphicsinfo.h"

class Texture {
public:
	Texture(SharedGraphicsInfo graphicsInfo) : graphicsInfo(graphicsInfo){}
	
	void load(const char* filename);

	void reset();

	VkImageView getTextureImageView() const { return textureImageView; }
	VkSampler getTextureSampler() const { return textureSampler; }

	~Texture();
private:

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void createTextureImageView();
	void createTextureSampler();

	SharedGraphicsInfo graphicsInfo;

	VkBuffer stagingBuffer; 
	VkDeviceMemory stagingBufferMemory; 

	VkImage textureImage; 
	VkDeviceMemory textureImageMemory; 
	
	VkImageView textureImageView;
	VkSampler textureSampler;
};