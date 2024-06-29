#pragma once
#include "GUI/object/imguiobject.h"
#include "GUI/light/imguilight.h"
#include <imgui.h>
#include <imfilebrowser.h>

class Scene;


struct MainMenu {
	Scene* scene;
	
	ImGui::FileBrowser fileDialog;

	MainMenu(Scene* scene) : scene(scene) {}

	void draw();

	void addObject(ImGuiObject* object);
	void addLight(ImGuiLight* light);
	void removeLight(ImGuiLight* light);
	void removeObject(ImGuiObject* object);

	~MainMenu();
protected:
	std::vector<ImGuiObject*> objects;
	std::vector<ImGuiLight*> lights;
};