#pragma once
#include "imnodes.h"
#include "material.h"

class OutputNode {
public:
	OutputNode(Material* material);
	void draw();
private:
	const int nodeId = 0;
	Material* material;
};