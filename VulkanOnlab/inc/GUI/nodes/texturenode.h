#pragma once
#include "GUI/nodes/node.h"
#include "vulkan/texture/texture.h"
#include "imgui.h"
#include <imfilebrowser.h>


struct TextureNode : Node {
	TextureNode(int id);

	virtual void draw();

	Texture* getTexture() { return texture; }

	void createTexture() { texture = new Texture(); }

	std::string getOutputShaderCode(int ouputId);

	std::string selectedTexturePath = " ";

	bool hasBeenAssigned = false;

	~TextureNode();
private:
	Texture* texture = nullptr;

	std::string selectedTexture = " ";

	ImGui::FileBrowser fileDialog;
};