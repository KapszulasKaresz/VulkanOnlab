#pragma once
#include "GUI/nodes/node.h"
#include "imgui.h"
#include <glm/glm.hpp>

struct PositionNode : Node {
	PositionNode(int id) : Node(id) {}

	void draw();

	glm::vec3 color = glm::vec3(0.0f);
private:

};