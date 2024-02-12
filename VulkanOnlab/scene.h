#pragma once
#include "object.h"
#include "camera.h"
#include "mainmenu.h"
#include <vector>


class Scene {
public:
	Camera camera;
	MainMenu* mainMenu;

	Scene() {}

	void buildScene(VkDevice& device, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue
		, VkExtent2D& swapChainExtent, VkRenderPass& renderPass, VkSurfaceKHR& surface,VkCommandPool& commandPool);

	void cleanup();

	void updateUniformBuffer(uint32_t currentImage);

	std::vector<Object*>& getObjects() { return objects; }
	void addLight();

	void deleteLight(Light* light);

	~Scene();
private:
	std::vector<Object*> objects;
	std::vector<Light*> lights;
};