#pragma once
#include <imgui.h>
#include "vulkan/object/object.h"
#include "GUI/transform/imguitransformation.h"
#include "GUI/transform/imguitranslation.h"
#include "GUI/transform/imguirotation.h"
#include "GUI/transform/imguiscale.h"
#include "vulkan/transform/translation.h"
#include "vulkan/transform/scale.h"
#include <vector>

class Scene;
class NodeEditor;
struct MainMenu;

struct ImGuiObject {
	int id;
	Object* object;
	Scene* scene;
	MainMenu* mainMenu;
	std::string name;

	ImGuiObject(Object* object, const char* name, Scene* scene, MainMenu* mainMenu);
		

	void remove(ImGuiTransformation* transformation);

	void draw();

	bool operator==(ImGuiObject& other) { return id == other.id; }

	~ImGuiObject();
private:
	static int rollingId;
	std::vector<ImGuiTransformation*> transformations;
};