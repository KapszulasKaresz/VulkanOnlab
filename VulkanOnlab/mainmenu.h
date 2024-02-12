#pragma once
#include <imgui.h>
#include "imguiobject.h"
#include "imguilight.h"


struct MainMenu {


	void draw();

	void addObject(ImGuiObject* object) { objects.push_back(object); }
	void addLight(ImGuiLight* light) { lights.push_back(light); }

	~MainMenu();
protected:
	std::vector<ImGuiObject*> objects;
	std::vector<ImGuiLight*> lights;
};