#pragma once
#include "vulkan/material/material.h"
#include <string>

class RenameWindow {
public:
	void draw();
	void open(Material* material);
private:
	bool display = false;
	Material* material;
	std::string currentName;
};