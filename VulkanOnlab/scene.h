#pragma once
#include "object.h"
#include "camera.h"
#include <vector>

struct MainMenu;

class Scene {
public:
	Camera camera;
	MainMenu* mainMenu;

	Scene(VkDevice& device, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue
		, VkExtent2D& swapChainExtent, VkRenderPass& renderPass, VkSurfaceKHR& surface,
		VkCommandPool& commandPool, VkDescriptorPool& descriptorPool, VkDescriptorSetLayout& globalDescriptorSetLayout, 
		std::vector<VkDescriptorSet>& globalDescriptorSets) :
	device(device), physicalDevice(physicalDevice), graphicsQueue(graphicsQueue), swapChainExtent(swapChainExtent), renderPass(renderPass),
	surface(surface), commandPool(commandPool), descriptorPool(descriptorPool),
	globalDescriptorSetLayout(globalDescriptorSetLayout), globalDescriptorSets(globalDescriptorSets){}

	void buildScene();

	void cleanup();

	void addObject(const char* filename, MainMenu* mainMenu);
	void removeObject(Object* object);

	void updateUniformBuffer(uint32_t currentImage);

	std::vector<Object*>& getObjects() { return objects; }
	std::vector<Light*>& getLights() { return lights; }
	void addLight();

	void deleteLight(Light* light);
	void drawMenu();

	~Scene();
private:
	std::vector<Object*> objects;
	std::vector<Light*> lights;


	VkDevice& device;
	VkPhysicalDevice& physicalDevice;
	VkQueue& graphicsQueue;
	VkExtent2D& swapChainExtent;
	VkRenderPass& renderPass; 
	VkSurfaceKHR& surface;
	VkCommandPool& commandPool;
	VkDescriptorPool& descriptorPool;
	VkDescriptorSetLayout& globalDescriptorSetLayout;
	std::vector<VkDescriptorSet>& globalDescriptorSets;
};