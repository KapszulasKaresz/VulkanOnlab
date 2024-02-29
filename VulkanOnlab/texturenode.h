#pragma once
#include "node.h"
#include "texture.h"
#include "imgui.h"
#include <imfilebrowser.h>


struct TextureNode : Node {
	TextureNode(int id, SharedGraphicsInfo graphInfo);

	void draw();

	Texture* getTexture() { return texture; }

	std::string selectedTexturePath = " ";

	bool hasBeenAssigned = false;

	~TextureNode();
private:
	Texture* texture = nullptr;
	SharedGraphicsInfo graphInfo;

	std::string selectedTexture = " ";

	ImGui::FileBrowser fileDialog;
};