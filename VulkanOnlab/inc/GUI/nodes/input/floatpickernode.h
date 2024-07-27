#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"
#include <glm/glm.hpp>

struct FloatNode : Node {
	FloatNode(int id) : Node(id) {}

	std::string getOutputShaderCode(int ouputId);
	void draw();

	float number = 0.0;
private:

}; 
