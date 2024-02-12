#pragma once
#include <imgui.h>
#include "object.h"

struct ImGuiObject {
	Object* object;
	std::string name;

	ImGuiObject(Object* object, const char* name) : object(object), name(name) {}

	void draw();
};