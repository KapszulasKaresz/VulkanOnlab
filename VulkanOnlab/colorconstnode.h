#pragma once
#include "node.h"
#include "imgui.h"
#include <glm/glm.hpp>

struct ColorConstNode : Node {
	ColorConstNode(int id) : Node(id){}

	void draw();

	glm::vec3 color = glm::vec3(0.0f);
private:
	
};