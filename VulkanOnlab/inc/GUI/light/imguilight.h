#pragma once
#include "imGuIZMOquat.h"
#include "vulkan/light/light.h"
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
	glm::vec3 dir;
	glm::vec3 updir = glm::vec3(0, 1, 0);
	bool directional = false;

	ImGuiLight(Light* light, Scene* scene, MainMenu* mainmenu);

	bool operator==(ImGuiLight& other);

	void draw();
};

