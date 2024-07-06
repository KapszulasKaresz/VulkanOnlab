#pragma once
#include "GUI/nodes/outputnode.h"
#include "imnodes.h"
#include "GUI/nodes/checkeredtexturenode.h"
#include "GUI/nodes/node.h"
#include <string>
#include <vector>
#include <utility>
#include "vulkan/material/material.h"
#include "GUI/nodes/mixernode.h"
#include "GUI/nodes/colorconstnode.h"
#include "GUI/nodes/maskingnode.h"
#include "GUI/nodes/invertednode.h"
#include "GUI/nodes/mathnode.h"
#include "GUI/nodes/positionnode.h"

class Object;

class NodeEditor {
public:
	NodeEditor(Material* material);
	
	void open(std::string& name);
	void close();
	void draw();
	void generateShaderCode();

	~NodeEditor();
private:
	enum RenderingMode : unsigned int {
		PBR, Phong
	};

	void deleteNode(int id);
	void newLinkToNodes(int startAttrib, int endAttrib);
	void deleteLinkFromNode(int startAttrib, int endAttrib);
	void resetEditor();

	RenderingMode renderingMode = Phong;

	OutputNode* outputNode;
	Material* material;

	std::vector<Node*> nodes;
	std::vector<TextureNode*> textureNodes;

	std::vector<std::pair<int, int>> links;

	int nodeId = 1;

	bool display = false;
	std::string name;
	std::string fragShaderName;
};