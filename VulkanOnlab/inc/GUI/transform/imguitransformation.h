#pragma once
#include <string>
#include <imgui.h>
#include "vulkan/transform/transformation.h"

struct ImGuiObject;

struct ImGuiTransformation {
	std::string name;
	ImGuiObject* parent;
	int id;
	static int rotatingId;

	ImGuiTransformation(std::string name, ImGuiObject* parent) : name(name), parent(parent) { id = rotatingId++; }
	virtual void draw() = 0;
	virtual Transformation* getTransform() = 0;
	void drawDelete();

	bool operator==(ImGuiTransformation& other) { return id == other.id; }
};