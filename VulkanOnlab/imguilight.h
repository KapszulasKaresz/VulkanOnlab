#pragma once
#include "light.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>


class Scene;
struct MainMenu;

struct ImGuiLight {
	static int rollingId;
	int id;
	Light* light;
	Scene* scene;
	MainMenu* mainmenu;
	bool directional = false;

	ImGuiLight(Light* light, Scene* scene, MainMenu* mainmenu);

	bool operator==(ImGuiLight& other);

	void draw();
};

