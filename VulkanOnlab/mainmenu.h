#pragma once
#include "imguiobject.h"
#include "imguilight.h"
#include <imgui.h>

class Scene;


struct MainMenu {
	Scene* scene;

	MainMenu(Scene* scene) : scene(scene) {}

	void draw();

	void addObject(ImGuiObject* object);
	void addLight(ImGuiLight* light);
	void removeLight(ImGuiLight* light);

	~MainMenu();
protected:
	std::vector<ImGuiObject*> objects;
	std::vector<ImGuiLight*> lights;
};