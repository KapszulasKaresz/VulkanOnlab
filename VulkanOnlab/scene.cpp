#include "scene.h"
#include "application.h"
#include "translation.h"
#include "imguiobject.h"

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
	Material* mat = new Material();
	mat->ka = glm::vec3(1.0f, 0.0f, 1.0f);
	mat->kd = glm::vec3(1.0f, 0.0f, 1.0f);
	mat->ks = glm::vec3(0.3f, 0.3f, 0.3f);
	mat->shininess = 15.0f;
	obj->material = mat;

	objects.push_back(obj);

	ImGuiObject* imObj = new ImGuiObject(obj, "test");

	mainMenu.addObject(imObj);
	lights.push_back(Light(glm::vec4(10.0f, 10.0f, 10.0f, 1.0f), glm::vec3(0.2f), glm::vec3(1)));

	ImGuiLight* imLight = new ImGuiLight(&(lights[0]));
	mainMenu.addLight(imLight);

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

		object->updateUniformBuffer(currentImage,camera, lights);
	}
}

Scene::~Scene()
{
	for (int i = 0; i < objects.size();i++) {
		delete objects[i];
	}
}
