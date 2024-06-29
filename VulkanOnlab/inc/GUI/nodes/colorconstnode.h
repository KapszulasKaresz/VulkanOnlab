#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"
#include <glm/glm.hpp>

struct ColorConstNode : Node {
	ColorConstNode(int id) : Node(id){}

	std::string getOutputShaderCode(int ouputId);
	void draw();

	glm::vec4 color = glm::vec4(0.0f);
private:
	
};