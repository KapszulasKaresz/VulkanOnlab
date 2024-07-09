#pragma once
#include "vulkan/texture/texture.h"
#include <array>
#include <string>

class Texture2D;

class Cubemap : public Texture {
public:	
	Cubemap();

	void load(const char* filename, bool generatePreview = false);

	void reset() override;

	VkImageView getTextureImageView() const override;
	VkSampler getTextureSampler() const override;

	int texWidth = 0;
	int texHeight = 0;
	int texChannels = 0;
	uint32_t mipLevels;

	std::array<Texture2D*, 6> previewTextures = {nullptr};

	/*
	0 pz
	1 nz
	2 py
	3 ny
	4 px
	5 nx
	*/
	const std::array<std::string, 6> sideNames = {
		"posx.hdr",
		"negx.hdr",
		"posy.hdr",
		"negy.hdr",
		"posz.hdr",
		"negz.hdr"
	};

	~Cubemap();
protected:
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t face);

	void createTextureImageView();
	void createTextureSampler();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	bool previews = false;

	VkDeviceSize imageSize;

	VkImageView textureImageView = VK_NULL_HANDLE;
	VkSampler textureSampler = VK_NULL_HANDLE;
};