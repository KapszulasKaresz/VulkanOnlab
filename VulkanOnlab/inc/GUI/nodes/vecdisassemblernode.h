#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"

template<size_t S>
struct VecDisassemblerNode : Node {
	VecDisassemblerNode(int id) : Node(id) {}

	std::string getOutputShaderCode(int ouputId);
	void draw();

private:
	const std::array<std::string, 4> components = { "x", "y", "z", "w" };
};