#pragma once
#include "imnodes.h"
#include "vulkan/material/material.h"
#include "GUI/nodes/output/outputnode.h"
#include "vulkan/material/materialPhong.h"

class OutputNodePhong :public OutputNode{
public:
	OutputNodePhong(MaterialPhong* material);
	Material* getMaterial();
	std::string getOutputShaderCode(int ouputId) override;
	std::string getShaderCodeUniforms() override;
	std::string getFunctionDefinitions() override;
	void draw();
	
private:
	MaterialPhong* material;
};