#pragma once
#include "GUI/nodes/outputnode.h"
#include "imnodes.h"
#include "GUI/nodes/checkeredtexturenode.h"
#include "GUI/nodes/node.h"
#include <string>
#include <vector>
#include <utility>
#include "vulkan/material/material.h"
#include "vulkan/sharedgraphicsinfo.h"
#include "GUI/nodes/mixernode.h"
#include "GUI/nodes/colorconstnode.h"
#include "GUI/nodes/maskingnode.h"
#include "GUI/nodes/invertednode.h"
#include "GUI/nodes/mathnode.h"
#include "GUI/nodes/positionnode.h"

class Object;

class NodeEditor {
public:
	NodeEditor(Material* material, Object* obejct);
	
	void open(std::string& name);
	void close();
	void draw();
	void generateShaderCode();

	~NodeEditor();
private:
	enum RenderingMode : unsigned int {
		PBR, Phong
	};

	std::string getColorInput(int id);
	void deleteNode(int id);
	void newLinkToNodes(int startAttrib, int endAttrib);
	void deleteLinkFromNode(int startAttrib, int endAttrib);

	RenderingMode renderingMode = Phong;

	OutputNode* outputNode;
	Material* material;
	Object* object;

	std::vector<Node*> nodes;
	std::vector<TextureNode*> textureNodes;
	std::vector<ColorConstNode*> colorConstNodes;
	std::vector<MixerNode*> mixerNodes;
	std::vector<MaskingNode*> maskingNodes;
	std::vector<InverterNode*> inverterNodes;
	std::vector<MathNode*> mathNodes;
	std::vector<PositionNode*> positionNodes;

	std::vector<std::pair<int, int>> links;

	int nodeId = 1;

	bool display = false;
	std::string name;
	std::string fragShaderName;

	SharedGraphicsInfo sharedGraphInfo;
};