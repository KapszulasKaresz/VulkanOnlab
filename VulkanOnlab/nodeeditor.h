#pragma once
#include "outputnode.h"
#include "imnodes.h"
#include "checkeredtexturenode.h"
#include "node.h"
#include <string>
#include <vector>
#include <utility>
#include "material.h"
#include "sharedgraphicsinfo.h"
#include "mixernode.h"
#include "colorconstnode.h"
#include "maskingnode.h"
#include "invertednode.h"
#include "mathnode.h"

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

	std::vector<std::pair<int, int>> links;

	int nodeId = 1;

	bool display = false;
	std::string name;
	std::string fragShaderName;

	SharedGraphicsInfo sharedGraphInfo;
};