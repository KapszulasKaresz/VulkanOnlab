#pragma once
#include "imnodes.h"
#include "vulkan/material/material.h"

class OutputNode {
public:
	OutputNode(Material* material) :material(material) {};
	int getId() { return nodeId; }
	Material* getMaterial() { return material; }
	virtual void draw() = 0;
	virtual ~OutputNode() {}
private:
	const int nodeId = 0;
	Material* material;
};