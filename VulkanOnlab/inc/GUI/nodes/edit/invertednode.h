#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"


struct InverterNode : Node {
	InverterNode(int id);

	std::string getOutputShaderCode(int ouputId);
	void draw();
private:
};