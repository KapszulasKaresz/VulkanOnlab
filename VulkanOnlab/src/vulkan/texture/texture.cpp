#include "vulkan/texture/texture.h"
#include "vulkan/application.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <vector>

int Texture::rollingId = 0;

void Texture::load(const char* filename)
{
	stbi_uc* pixels = stbi_load(filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	Application::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data; 
	vkMapMemory((Application::device), stagingBufferMemory, 0, imageSize, 0, &data); 
	memcpy(data, pixels, static_cast<size_t>(imageSize)); 
	vkUnmapMemory((Application::device), stagingBufferMemory);

	stbi_image_free(pixels);

	createImage(texWidth, texHeight, mipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels); 
	copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight)); 

	vkDestroyBuffer((Application::device), stagingBuffer, nullptr);
	vkFreeMemory((Application::device), stagingBufferMemory, nullptr);
	generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
	createTextureImageView();
	createTextureSampler();
}

void Texture::generateCheckered(glm::vec3& color1, glm::vec3& color2, float scale)
{
	texWidth = 1024;
	texHeight = 1024;

	VkDeviceSize imageSize = texWidth * texHeight * 4;
	std::vector<stbi_uc> pixels(imageSize);

	for (int i = 0; i < texWidth; i++) {
		for (int j = 0; j < texHeight; j++) {
			if ((i / (int)(texWidth / scale) % 2 == 0) ^ (j / (int)(texHeight / scale) % 2 == 1)) {
				pixels[(j * texWidth + i) * 4 + 0] = ((stbi_uc)(255 * color1.r));
				pixels[(j * texWidth + i) * 4 + 1] = ((stbi_uc)(255 * color1.g));
				pixels[(j * texWidth + i) * 4 + 2] = ((stbi_uc)(255 * color1.b));
				pixels[(j * texWidth + i) * 4 + 3] = ((stbi_uc)255);
			}
			else {
				pixels[(j * texWidth + i) * 4 + 0] = ((stbi_uc)(255 * color2.r));
				pixels[(j * texWidth + i) * 4 + 1] = ((stbi_uc)(255 * color2.g));
				pixels[(j * texWidth + i) * 4 + 2] = ((stbi_uc)(255 * color2.b));
				pixels[(j * texWidth + i) * 4 + 3] = ((stbi_uc)255);
			}
		}
	}

	mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

	Application::createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory((Application::device), stagingBufferMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels.data(), static_cast<size_t>(imageSize));
	vkUnmapMemory((Application::device), stagingBufferMemory);


	createImage(texWidth, texHeight, mipLevels, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

	transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
	copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));

	vkDestroyBuffer((Application::device), stagingBuffer, nullptr);
	vkFreeMemory((Application::device), stagingBufferMemory, nullptr);
	generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
	createTextureImageView();
	createTextureSampler();

}

void Texture::reset()
{
	vkDeviceWaitIdle((Application::device));

	vkDestroySampler((Application::device), textureSampler, nullptr);
	vkDestroyImageView((Application::device), textureImageView, nullptr);

	vkDestroyImage((Application::device), textureImage, nullptr);
	vkFreeMemory((Application::device), textureImageMemory, nullptr);

	textureImageView = VK_NULL_HANDLE;
	textureSampler = VK_NULL_HANDLE;
}

VkImageView Texture::getTextureImageView() const
{
	if (textureImageView == VK_NULL_HANDLE) {
		throw std::runtime_error("texture hasn't been loaded");
	}
	return textureImageView;
}

VkSampler Texture::getTextureSampler() const
{
	if (textureSampler == VK_NULL_HANDLE) {
		throw std::runtime_error("texture hasn't been loaded");
	}
	return textureSampler;
}

Texture::~Texture()
{
}

void Texture::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = mipLevels;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage((Application::device), &imageInfo, nullptr, &image) != VK_SUCCESS) {
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements((Application::device), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = Application::findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory((Application::device), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory((Application::device), image, imageMemory, 0);
}

void Texture::generateMipmaps(VkImage image, VkFormat imageFormat,int32_t texWidth, int32_t texHeight, uint32_t mipLevels)
{

	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties((Application::physicalDevice), imageFormat, &formatProperties);

	if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
		throw std::runtime_error("texture image format does not support linear blitting!");
	}

	VkCommandBuffer commandBuffer = Application::beginSingleTimeCommands();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = texWidth;
	int32_t mipHeight = texHeight;

	for (uint32_t i = 1; i < mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i -1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
			image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1, &blit,
			VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
			0, nullptr,
			0, nullptr,
			1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
		VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
		0, nullptr,
		0, nullptr,
		1, &barrier);

	Application::endSingleTimeCommands(commandBuffer);
}

void Texture::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels)
{
	VkCommandBuffer commandBuffer = Application::beginSingleTimeCommands(); 
		
		VkImageMemoryBarrier barrier{}; 
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER; 
		barrier.oldLayout = oldLayout; 
		barrier.newLayout = newLayout; 
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = image; 
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; 
		barrier.subresourceRange.baseMipLevel = 0; 
		barrier.subresourceRange.levelCount = mipLevels; 
		barrier.subresourceRange.baseArrayLayer = 0; 
		barrier.subresourceRange.layerCount = 1; 
	
		VkPipelineStageFlags sourceStage; 
		VkPipelineStageFlags destinationStage; 
		 
		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
			barrier.srcAccessMask = 0; 
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; 

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT; 
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT; 
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; 
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT; 

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT; 
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; 
		}
		else {
			throw std::invalid_argument("unsupported layout transition!");
		}

		vkCmdPipelineBarrier( 
			commandBuffer, 
			sourceStage, destinationStage, 
			0,
			0, nullptr,
			0, nullptr,
			1, &barrier 
		);


	 
	Application::endSingleTimeCommands(commandBuffer); 
}

void Texture::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
	VkCommandBuffer commandBuffer = Application::beginSingleTimeCommands(); 
		VkBufferImageCopy region{};
		region.bufferOffset = 0; 
		region.bufferRowLength = 0; 
		region.bufferImageHeight = 0; 

		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0; 
		region.imageSubresource.layerCount = 1; 

		region.imageOffset = { 0, 0, 0 };
		region.imageExtent = {
			width, 
			height,
			1
		};

		vkCmdCopyBufferToImage(
			commandBuffer,
			buffer,
			image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&region
		);

	Application::endSingleTimeCommands(commandBuffer);  
}


void Texture::createTextureImageView()
{
	VkImageViewCreateInfo viewInfo{}; 
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO; 
	viewInfo.image = textureImage; 
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D; 
	viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB; 
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT; 
	viewInfo.subresourceRange.baseMipLevel = 0; 
	viewInfo.subresourceRange.levelCount = mipLevels; 
	viewInfo.subresourceRange.baseArrayLayer = 0; 
	viewInfo.subresourceRange.layerCount = 1; 

	if (vkCreateImageView((Application::device), &viewInfo, nullptr, &textureImageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}
}

void Texture::createTextureSampler()
{
	VkSamplerCreateInfo samplerInfo{};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = VK_FILTER_LINEAR; 
	samplerInfo.minFilter = VK_FILTER_LINEAR; 
	samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; 
	samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT; 
	samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInfo.anisotropyEnable = VK_TRUE; 

	VkPhysicalDeviceProperties properties{};  
	vkGetPhysicalDeviceProperties((Application::physicalDevice), &properties);
	samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy; 
	samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	samplerInfo.unnormalizedCoordinates = VK_FALSE; 
	samplerInfo.compareEnable = VK_FALSE; 
	samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS; 
	samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR; 
	samplerInfo.minLod = 0;
	samplerInfo.maxLod = static_cast<float>(mipLevels);
	samplerInfo.mipLodBias = 0.0f; // Optional
	
	if(vkCreateSampler((Application::device), &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture sampler!");
	}
}

