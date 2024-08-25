#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include "vulkan/texture/texture.h"


class Texture2D : public Texture{
public:
	Texture2D() : Texture() {}
	
	void load(const char* filename, VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB);

	void generateCheckered(glm::vec3& color1, glm::vec3& color2, float scale);

	void reset() override;

	VkImageView getTextureImageView() const override;
	VkSampler getTextureSampler() const override;

	int texWidth = 0;
	int texHeight = 0;
	int texChannels = 0;

	~Texture2D();
protected:
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void createTextureImageView();
	void createTextureSampler();

	VkBuffer stagingBuffer; 
	VkDeviceMemory stagingBufferMemory; 

	uint32_t mipLevels;
	VkImage textureImage; 
	VkDeviceMemory textureImageMemory; 

	VkImageView textureImageView = VK_NULL_HANDLE;
	VkSampler textureSampler = VK_NULL_HANDLE;
};