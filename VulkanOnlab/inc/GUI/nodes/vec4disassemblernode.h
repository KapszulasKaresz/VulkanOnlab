#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"

struct Vec4DisassemblerNode : Node {
	Vec4DisassemblerNode(int id) : Node(id) {}

	std::string getOutputShaderCode(int ouputId);
	void draw();

private:

};