#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include "sharedgraphicsinfo.h"

class Texture {
public:
	Texture(SharedGraphicsInfo graphicsInfo) : graphicsInfo(graphicsInfo){}
	
	void load(const char* filename);

	void reset();

	VkImageView getTextureImageView() const;
	VkSampler getTextureSampler() const;

	~Texture();
private:

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	VkCommandBuffer beginSingleTimeCommands();
	void endSingleTimeCommands(VkCommandBuffer commandBuffer);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void createTextureImageView();
	void createTextureSampler();

	SharedGraphicsInfo graphicsInfo;

	VkBuffer stagingBuffer; 
	VkDeviceMemory stagingBufferMemory; 

	uint32_t mipLevels;
	VkImage textureImage; 
	VkDeviceMemory textureImageMemory; 
	
	VkImageView textureImageView = VK_NULL_HANDLE;
	VkSampler textureSampler = VK_NULL_HANDLE;
};