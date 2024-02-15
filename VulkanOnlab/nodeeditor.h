#pragma once
#include "imnodes.h"
#include <string>


class NodeEditor {
public:
	void open(std::string& name);
	void close();
	void draw();
private:
	bool display = false;
	std::string name;
};