#pragma once
#include "imnodes.h"
#include "vulkan/material/material.h"
#include "GUI/nodes/node.h"

class OutputNode :public Node {
public:
	OutputNode() :Node(0) {};
	virtual Material* getMaterial() = 0;
	virtual void draw() = 0;
	virtual std::string getShaderCodeUniforms() = 0;
	virtual std::string getOutputShaderCode(int ouputId) = 0;
	virtual ~OutputNode() {}
private:
};