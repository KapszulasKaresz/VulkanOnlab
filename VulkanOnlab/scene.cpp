#include "scene.h"
#include "application.h"
#include "translation.h"

void Scene::buildScene(VkDevice& device, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue
	, VkExtent2D& swapChainExtent, VkRenderPass& renderPass, VkSurfaceKHR& surface, VkCommandPool& commandPool)
{
	camera.asp = (float)Application::WIDTH / Application::HEIGHT;
	camera.fov = glm::radians(45.0f);
	camera.fp = 200.0f;
	camera.np = 0.1f;
	camera.wForward = glm::normalize(glm::vec3(0.0f)- glm::vec3(5.0f, 5.0f, 5.0f));
	camera.wVup = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.wEye = glm::vec3(5.0f, 5.0f, 5.0f);

	Object* obj = new Object(device, physicalDevice, graphicsQueue, swapChainExtent, renderPass, surface, commandPool);
	obj->create();
	objects.push_back(obj);

	/*Object* obj2 = new Object(device, physicalDevice, graphicsQueue, swapChainExtent, renderPass, surface, commandPool);
	obj2->create();
	obj2->transformations.push_back(new Translation(glm::vec3(0.0f, 0.0f, 4.0f)));
	objects.push_back(obj2);*/
}

void Scene::cleanup()
{
	for (Object* object : objects) {
		object->cleanup();
	}
}

void Scene::updateUniformBuffer(uint32_t currentImage)
{
	for (Object* object : objects) {

		object->updateUniformBuffer(currentImage,camera);
	}
}

Scene::~Scene()
{
	for (int i = 0; i < objects.size();i++) {
		delete objects[i];
	}
}
