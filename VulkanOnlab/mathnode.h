#pragma once
#pragma once
#include "node.h"
#include "imgui.h"
#include <string>

struct MathNode : Node {
	MathNode(int id);

	void draw();

	std::string getOperator();
private:
	int selected = 0;
};