#pragma once
#include "outputnode.h"
#include "imnodes.h"
#include "texturenode.h"
#include "node.h"
#include <string>
#include <vector>
#include <utility>
#include "material.h"
#include "sharedgraphicsinfo.h"
#include "mixernode.h"
#include "colorconstnode.h"

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

	std::string getColorInput(int id);

	RenderingMode renderingMode = Phong;

	OutputNode* outputNode;
	Material* material;

	std::vector<Node*> nodes;
	std::vector<TextureNode*> textureNodes;
	std::vector<ColorConstNode*> colorConstNodes;
	std::vector<MixerNode*> mixerNodes;

	std::vector<std::pair<int, int>> links;

	int nodeId = 1;

	bool display = false;
	std::string name;

	SharedGraphicsInfo sharedGraphInfo;
};