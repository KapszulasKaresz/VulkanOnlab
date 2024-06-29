#pragma once
#include "imnodes.h"
#include "vulkan/material/material.h"
#include "GUI/nodes/node.h"

class OutputNode :public Node {
public:
	OutputNode(Material* material) :Node(0), material(material) {};
	Material* getMaterial() { return material; }
	virtual void draw() = 0;
	virtual ~OutputNode() {}
private:
	Material* material;
};