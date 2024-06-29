#pragma once
#include "imnodes.h"
#include "vulkan/material/material.h"
#include "GUI/nodes/outputnode.h"

class OutputNodePhong :public OutputNode{
public:
	OutputNodePhong(Material* material);
	std::string getOutputShaderCode(int ouputId);
	void draw();
private:
};