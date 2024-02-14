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

struct ImGuiObject {
	Object* object;
	std::string name;

	ImGuiObject(Object* object, const char* name) : object(object), name(name) {}

	void remove(ImGuiTransformation* transformation);

	void draw();

	~ImGuiObject();
private:
	std::vector<ImGuiTransformation*> transformations;
};