#pragma once
#include "imnodes.h"
#include <glm/glm.hpp>
#include "GUI/nodes/outputnode.h"
#include "vulkan/material/materialPBR.h"

class OutputNodePBR :public OutputNode{
public:
	OutputNodePBR(MaterialPBR* material);
	Material* getMaterial() { return nullptr; }
	std::string getShaderCodeUniforms();
	std::string getOutputShaderCode(int ouputId) override;
	void draw();
private:
	MaterialPBR* material;
};