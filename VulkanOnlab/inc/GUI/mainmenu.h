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
	
	ImGui::FileBrowser fileDialogObj;
	ImGui::FileBrowser fileDialogGLTF;

	MainMenu(Scene* scene) : scene(scene) {}

	void draw();

	void addLight(ImGuiLight* light);
	void removeLight(ImGuiLight* light);

	~MainMenu();
protected:
	std::vector<ImGuiLight*> lights;

	StatWindow statWindow;
	ImGuiMaterial materialDrawer;
};