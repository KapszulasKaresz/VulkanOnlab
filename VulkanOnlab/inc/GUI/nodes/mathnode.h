#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"
#include <string>

struct MathNode : Node {
	MathNode(int id);

	void draw();

	std::string getOutputShaderCode(int ouputId);

	std::string getOperator();
private:
	int selected = 0;
};