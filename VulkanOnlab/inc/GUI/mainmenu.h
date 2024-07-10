#pragma once
#include "GUI/object/imguiobject.h"
#include "GUI/light/imguilight.h"
#include "GUI/material/imguimaterial.h"
#include <imgui.h>
#include <imfilebrowser.h>
#include "GUI/stats.h"

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

	StatWindow statWindow;
	ImGuiMaterial materialDrawer;
};