#pragma once
#include "vulkan/object/object.h"
#include "vulkan/camera.h"
#include "vulkan/framework/accelerationstructure.h"
#include <vector>

struct MainMenu;

namespace tinygltf {
	class Model;
}

class Scene {
public:
	Camera camera;
	MainMenu* mainMenu;

	Scene() {}

	void buildScene();

	void cleanup();

	void loadDummData();

	void addObject(const char* filename, MainMenu* mainMenu);
	void removeObject(Object* object);

	bool loadGLTFScene(const char* filename, MainMenu* mainMenu);

	void updateUniformBuffer(uint32_t currentImage);

	std::vector<Object*>& getObjects() { return objects; }
	std::vector<Light*>& getLights() { return lights; }
	void addLight();

	void deleteLight(Light* light);
	void drawMenu();

	void updateAS();

	void createTopLevelAccelerationStructure();

	AccelerationStructure* topLevelAS = nullptr;

	bool topLevelASChanged = false;

	~Scene();
private:
	void loadGLTFMaterials(tinygltf::Model* gltfModel);

	bool deleteObjectWithAS = false;
	void createASInstanceBuffer();
	VkBuffer asInstanceBuffer = VK_NULL_HANDLE;
	VkDeviceMemory asInstanceBufferMemory;

	std::vector<Object*> objects;
	std::vector<Light*> lights;
};