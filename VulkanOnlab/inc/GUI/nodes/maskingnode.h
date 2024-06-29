#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"


struct MaskingNode : Node {
	MaskingNode(int id);

	std::string getOutputShaderCode(int ouputId);

	void draw();
private:
};