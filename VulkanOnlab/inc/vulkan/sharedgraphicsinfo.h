#pragma once
#include <vulkan/vulkan.h>

struct SharedGraphicsInfo {
	VkDevice* device;
	VkPhysicalDevice* physicalDevice;
	VkCommandPool* commandPool;
	VkQueue* graphicsQueue;
};