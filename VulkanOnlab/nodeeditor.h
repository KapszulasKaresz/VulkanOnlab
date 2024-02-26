#pragma once
#include "outputnode.h"
#include "imnodes.h"
#include "texturenode.h"
#include "node.h"
#include <string>
#include <vector>
#include "material.h"
#include "sharedgraphicsinfo.h"

class NodeEditor {
public:
	NodeEditor(Material* material);
	
	void open(std::string& name);
	void close();
	void draw();

	~NodeEditor();
private:
	OutputNode* outputNode;
	Material* material;

	std::vector<TextureNode*> nodes;

	int nodeId = 1;

	bool display = false;
	std::string name;

	SharedGraphicsInfo sharedGraphInfo;
};