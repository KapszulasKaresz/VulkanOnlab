#pragma once
#include "GUI/nodes/node.h"
#include "vulkan/texture/texture2D.h"
#include "imgui.h"
#include <imfilebrowser.h>


struct TextureNode : Node {
	TextureNode(int id);

	virtual void draw();

	Texture2D* getTexture() { return texture; }

	void createTexture() { texture = new Texture2D(); }
	void loadTexture(const char* filename, const char* name, VkFormat imageFormat = VK_FORMAT_R8G8B8A8_SRGB);

	std::string getOutputShaderCode(int ouputId);

	std::string selectedTexturePath = " ";

	bool hasBeenAssigned = false;

	~TextureNode();
private:
	Texture2D* texture = nullptr;

	std::string selectedTexture = " ";

	ImGui::FileBrowser fileDialog;
};