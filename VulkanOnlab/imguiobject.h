#pragma once
#include <imgui.h>
#include "object.h"
#include "imguitransformation.h"
#include "imguitranslation.h"
#include "imguirotation.h"
#include "imguiscale.h"
#include "translation.h"
#include "scale.h"
#include "imguirotation.h"
#include <vector>

class Scene;
class NodeEditor;
struct MainMenu;

struct ImGuiObject {
	int id;
	Object* object;
	Scene* scene;
	MainMenu* mainMenu;
	NodeEditor* nodeEditor = nullptr;
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