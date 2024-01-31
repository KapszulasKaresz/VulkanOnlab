#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stdexcept>
#include <vulkan/vulkan.h>

class Texture {
public:
	Texture(VkDevice* device) : device(device) {}
	
	void load(const char* filename);

	void reset();

	~Texture();
private:
	VkDevice* device;

	VkBuffer stagingBuffer; 
	VkDeviceMemory stagingBufferMemory; 
};