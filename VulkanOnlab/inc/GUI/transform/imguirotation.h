#pragma once
#include "imGuIZMOquat.h"
#include "GUI/transform/imguitransformation.h"
#include "vulkan/transform/rotation.h"

struct ImGuiObject;

struct ImGuiRotation : ImGuiTransformation {
	Rotation* rotation;

	ImGuiRotation(Rotation* rotation, ImGuiObject* parent) : rotation(rotation), ImGuiTransformation("Rotation", parent) {}
	Transformation* getTransform() { return rotation; }
	void draw();
};