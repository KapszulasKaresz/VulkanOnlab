#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"
#include <glm/glm.hpp>

struct PositionNode : Node {
	PositionNode(int id) : Node(id) {}

	std::string getOutputShaderCode(int ouputId);

	void draw();
private:

};