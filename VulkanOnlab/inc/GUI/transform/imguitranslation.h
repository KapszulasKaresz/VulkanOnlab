#pragma once
#include "imGuIZMOquat.h"
#include "GUI/transform/imguitransformation.h"
#include "vulkan/transform/translation.h"

struct ImGuiObject;

struct ImGuiTranslation : ImGuiTransformation {
	Translation* translation;

	ImGuiTranslation(Translation* translation, ImGuiObject* parent) : translation(translation), ImGuiTransformation("Translation", parent) {}
	Transformation* getTransform() { return translation; }
	void draw();
};