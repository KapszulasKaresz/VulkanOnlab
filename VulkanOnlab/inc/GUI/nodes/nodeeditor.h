#pragma once
#include "GUI/nodes/output/outputnode.h"
#include "imnodes.h"
#include "GUI/nodes/input/checkeredtexturenode.h"
#include "GUI/nodes/node.h"
#include <string>
#include <vector>
#include <utility>
#include "vulkan/material/material.h"
#include "GUI/nodes/edit/mixernode.h"
#include "GUI/nodes/input/colorconstnode.h"
#include "GUI/nodes/edit/maskingnode.h"
#include "GUI/nodes/edit/invertednode.h"
#include "GUI/nodes/edit/mathnode.h"
#include "GUI/nodes/input/positionnode.h"
#include <shaderc/shaderc.hpp> 
#include <filesystem>

class Object;

class NodeEditor {
public:
	NodeEditor(Material* material);
	
	void open(std::string& name);
	void close();
	void draw();
	shaderc::SpvCompilationResult generateShaderCode();
	void addAlbedoTexture(std::filesystem::path path);
	void addNormalTexture(std::filesystem::path path);
	void addMetallicRoughnessTexture(std::filesystem::path path);
	void forceApply();

	~NodeEditor();
private:
	enum RenderingMode : unsigned int {
		PBR, Phong, EnvMap
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

	shaderc::SpvCompilationResult shaderCompilationResult;

	int nodeId = 1;

	bool display = false;
	std::string name;
	std::string fragShaderName;
};