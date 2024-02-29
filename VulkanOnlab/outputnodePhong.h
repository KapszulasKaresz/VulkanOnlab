#pragma once
#include "imnodes.h"
#include "material.h"
#include "outputnode.h"

class OutputNodePhong :public OutputNode{
public:
	OutputNodePhong(Material* material);
	void draw();
private:
};