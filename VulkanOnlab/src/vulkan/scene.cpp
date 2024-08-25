#include "vulkan/scene.h"
#include "vulkan/application.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include <tinygltf/tiny_gltf.h>
#include "GUI/mainmenu.h"

void Scene::buildScene()
{
	camera.asp = (float)Application::WIDTH / Application::HEIGHT;
	camera.fov = glm::radians(45.0f);
	camera.fp = 200.0f;
	camera.np = 0.1f;
	camera.wForward = glm::normalize(glm::vec3(0.0f) - glm::vec3(5.0f, 5.0f, 5.0f));
	camera.wVup = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.wEye = glm::vec3(5.0f, 5.0f, 5.0f);
	mainMenu = new MainMenu(this);
	createTopLevelAccelerationStructure();
}

void Scene::cleanup()
{
	for (Object* object : objects) {
		object->cleanup();
	}

	if (topLevelAS != nullptr) {
		delete topLevelAS;
	}
	delete mainMenu;

	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}

	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}

	vkDestroyBuffer(Application::device, asInstanceBuffer, nullptr);
	vkFreeMemory(Application::device, asInstanceBufferMemory, nullptr);

	vkDestroyDescriptorSetLayout(Application::device, Object::descriptorSetLayout, nullptr);
}

void Scene::loadDummData()
{
	//Object* obj = new Object();
	//obj->create("res/models/icosphere.obj");

	//objects.push_back(obj);

	//topLevelASChanged = true;
	//createASInstanceBuffer();
	//topLevelAS->addInstanceGeometry(asInstanceBuffer, 1);
	//topLevelAS->build();

	//ImGuiObject* imObj = new ImGuiObject(obj, "res/models/cube.obj", this, mainMenu);
	//mainMenu->addObject(imObj);
}

void Scene::addObject(const char* filename, MainMenu* mainMenu)
{
	Object* obj = new Object();
	obj->create(filename);

	objects.push_back(obj);

	ImGuiObject* imObj = new ImGuiObject(obj, filename, this, mainMenu);
	mainMenu->addObject(imObj);
}

void Scene::removeObject(Object* object)
{
	if (object->hasAccelerationStructure) {
		deleteObjectWithAS = true;
	}
	for (int i = 0; i < objects.size(); i++) {
		if (*(objects[i]) == *object) {
			vkDeviceWaitIdle(Application::device);
			object->cleanup();
			delete objects[i];
			objects.erase(objects.begin() + i);
			break;
		}
	}
}

bool Scene::loadGLTFScene(const char* filename, MainMenu* mainMenu)
{
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	tinygltf::Model model;

	bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cout << "ERR: " << err << std::endl;
	}

	if (!res)
		std::cout << "Failed to load glTF: " << filename << std::endl;
	else
		std::cout << "Loaded glTF: " << filename << std::endl;

	for (int i = 0; i < model.meshes.size(); i++) {
		for (int j = 0; j < model.meshes[i].primitives.size(); j++) {
			Object* obj = new Object();
			obj->create(&(model.meshes[i].primitives[j]), &model);

			objects.push_back(obj);

			ImGuiObject* imObj = new ImGuiObject(obj, model.meshes[i].name.c_str(), this, mainMenu);
			mainMenu->addObject(imObj);
		}
	}

	return res;
}

void Scene::updateUniformBuffer(uint32_t currentImage)
{
	for (Object* object : objects) {

		object->updateUniformBuffer(currentImage, camera, lights);
	}
}

void Scene::addLight()
{
	if (lights.size() < MAX_LIGHTS) {
		lights.push_back(new Light(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.2f), glm::vec3(1)));
		ImGuiLight* imLight = new ImGuiLight(lights[lights.size() - 1], this, mainMenu);
		mainMenu->addLight(imLight);
	}
}

void Scene::deleteLight(Light* light)
{
	for (int i = 0; i < lights.size(); i++) {
		if (*(lights[i]) == *light) {
			delete lights[i];
			lights.erase(lights.begin() + i);
			break;
		}
	}
}

void Scene::drawMenu()
{
	mainMenu->draw();

}

void Scene::updateAS()
{
	int updateCount = 0;
	int objectsWithAS = 0;
	for (auto object : objects) {
		object->checkTransformationUpdate();
		if (object->hasAccelerationStructure) {
			objectsWithAS += 1;
			if (object->accelerationStructureDirty) {
				updateCount += 1;
				object->accelerationStructureDirty = false;
			}
		}
	}

	if (updateCount > 0 || deleteObjectWithAS) {
		topLevelASChanged = true;
		deleteObjectWithAS = false;
		if (objectsWithAS > 0) {
			createASInstanceBuffer();

			if (topLevelAS->isEmpty()) {
				topLevelAS->addInstanceGeometry(asInstanceBuffer, objects.size());
			}
			else {
				topLevelAS->updateInstanceGeometry(0, asInstanceBuffer, objects.size());
			}
			topLevelAS->build();
		}
	}
}

void Scene::createTopLevelAccelerationStructure()
{
	topLevelAS = new AccelerationStructure(Application::device, VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR);
}

Scene::~Scene()
{

}

void Scene::createASInstanceBuffer()
{
	std::vector<VkAccelerationStructureInstanceKHR> instances;
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->hasAccelerationStructure) {
			instances.push_back(objects[i]->instance);
		}
	}

	VkDeviceSize instancesSize = instances.size() * sizeof(VkAccelerationStructureInstanceKHR);

	if (asInstanceBuffer != VK_NULL_HANDLE) {
		vkDestroyBuffer(Application::device, asInstanceBuffer, nullptr);
		vkFreeMemory(Application::device, asInstanceBufferMemory, nullptr);
	}

	Application::createBuffer(instancesSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, asInstanceBuffer, asInstanceBufferMemory);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Application::createBuffer(instancesSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(
		Application::device, stagingBufferMemory, 0, instancesSize, 0, &data);
	memcpy(data, instances.data(), (size_t)instancesSize);
	vkUnmapMemory(Application::device, stagingBufferMemory);

	Application::copyBuffer(stagingBuffer, asInstanceBuffer, instancesSize);

	vkDestroyBuffer(Application::device, stagingBuffer, nullptr);
	vkFreeMemory(Application::device, stagingBufferMemory, nullptr);
}
