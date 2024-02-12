#pragma once
#include "light.h"


struct ImGuiLight {
	Light* light;
	bool directional = false;

	ImGuiLight(Light* light) : light(light) {}

	void draw();
};