#pragma once
#include <vulkan/vulkan.hpp>

struct SharedGraphicsInfo {
	VkDevice* device;
	VkPhysicalDevice* physicalDevice;
	VkCommandPool* commandPool;
	VkQueue* graphicsQueue;
};