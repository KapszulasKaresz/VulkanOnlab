#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"


struct MixerNode : Node {
	MixerNode(int id);

	float getMix() { return mix; }
	std::string getOutputShaderCode(int ouputId);
	void draw();
private:
	float mix = 0.5f;
};