#pragma once
#include "object.h"
#include "camera.h"
#include <vector>


class Scene {
public:
	Camera camera;

	Scene() {}

	void buildScene(VkDevice& device, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue
		, VkExtent2D& swapChainExtent, VkRenderPass& renderPass, VkSurfaceKHR& surface,VkCommandPool& commandPool);

	void cleanup();

	void updateUniformBuffer(uint32_t currentImage);

	std::vector<Object*>& getObjects() { return objects; }

private:
	std::vector<Object*> objects;
};