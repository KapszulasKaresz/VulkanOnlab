#pragma once
#include "imGuIZMOquat.h"
#include "imguitransformation.h"
#include "translation.h"

struct ImGuiObject;

struct ImGuiTranslation : ImGuiTransformation {
	Translation* translation;

	ImGuiTranslation(Translation* translation, ImGuiObject* parent) : translation(translation), ImGuiTransformation("Translation", parent) {}
	Transformation* getTransform() { return translation; }
	void draw();
};