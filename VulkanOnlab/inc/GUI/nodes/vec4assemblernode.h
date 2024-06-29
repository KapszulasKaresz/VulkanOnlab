#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"

struct Vec4AssemblerNode : Node {
	Vec4AssemblerNode(int id) : Node(id) {}

	std::string getOutputShaderCode(int ouputId);
	void draw();

private:

};