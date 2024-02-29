#pragma once
#include "node.h"
#include "imgui.h"


struct MixerNode : Node {
	MixerNode(int id);

	float getMix() { return mix; }
	void draw();
private:
	float mix = 0.5f;
};