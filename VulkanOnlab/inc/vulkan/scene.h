#pragma once
#include "vulkan/object/object.h"
#include "vulkan/camera.h"
#include "vulkan/framework/accelerationstructure.h"
#include "vulkan/object/node.h"
#include <vector>
#include <filesystem>

struct MainMenu;

namespace tinygltf {
	class Model;
	class Node;
}

class Scene {
public:
	Camera camera;
	MainMenu* mainMenu;

	Scene() {}

	void buildScene();

	void cleanup();

	void loadDummData();

	bool loadGLTFScene(std::filesystem::path path, MainMenu* mainMenu);

	void updateUniformBuffer(uint32_t currentImage);

	std::vector<Light*>& getLights() { return lights; }
	void addLight();

	void deleteLight(Light* light);
	void drawMenu();

	void updateAS();

	void createTopLevelAccelerationStructure();

	AccelerationStructure* topLevelAS = nullptr;
	std::vector<RenderNode*>& getRootNodes();

	std::vector<Object*> getObjects();

	bool topLevelASChanged = false;

	~Scene();
private:
	void loadGLTFMaterials(std::filesystem::path path, tinygltf::Model* gltfModel);

	void setTransforms(RenderNode* renderNode, tinygltf::Node* node);

	bool deleteObjectWithAS = false;
	void createASInstanceBuffer();
	VkBuffer asInstanceBuffer = VK_NULL_HANDLE;
	VkDeviceMemory asInstanceBufferMemory;

	std::vector<RenderNode*> rootNodes;
	std::vector<Light*> lights;
};