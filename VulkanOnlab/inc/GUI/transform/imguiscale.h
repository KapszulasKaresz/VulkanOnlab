#pragma once
#include "imGuIZMOquat.h"
#include "GUI/transform/imguitransformation.h"
#include "vulkan/transform/scale.h"

struct ImGuiObject;

struct ImGuiScale : ImGuiTransformation {
	Scale* scale;
	glm::vec3 updir = glm::vec3(0.0f, 1.0f, 0.0f);

	ImGuiScale(Scale* scale, ImGuiObject* parent) : scale(scale), ImGuiTransformation("Scale", parent) {}
	Transformation* getTransform() { return scale; }
	void draw();
};