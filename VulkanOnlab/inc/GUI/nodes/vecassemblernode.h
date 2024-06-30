#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"
#include "imnodes.h"
#include <array>

template<size_t S>
struct VecAssemblerNode : Node {
	 VecAssemblerNode(int id) : Node(id) {}

	 std::string getOutputShaderCode(int ouputId);
	 void draw();

private:
	const std::array<std::string, 4> components = {"x", "y", "z", "w"};
};

