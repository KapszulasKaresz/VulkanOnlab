#pragma once
#include "imnodes.h"
#include "vulkan/material/material.h"
#include "GUI/nodes/outputnode.h"
#include "vulkan/material/materialPhong.h"

class OutputNodePhong :public OutputNode{
public:
	OutputNodePhong(MaterialPhong* material);
	Material* getMaterial();
	std::string getOutputShaderCode(int ouputId);
	std::string getShaderCodeUniforms();
	void draw();
private:
	MaterialPhong* material;
};