#pragma once
#include "vulkan/object/object.h"
#include "vulkan/camera.h"
#include <vector>

struct MainMenu;

class Scene {
public:
	Camera camera;
	MainMenu* mainMenu;

	Scene() {}

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
};