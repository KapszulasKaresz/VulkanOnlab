#pragma once
#include "imnodes.h"
#include <glm/glm.hpp>
#include "GUI/nodes/outputnode.h"
#include "vulkan/material/materialPBR.h"
#include <imfilebrowser.h>

class OutputNodePBR :public OutputNode{
public:
	OutputNodePBR(MaterialPBR* material);
	Material* getMaterial() { return nullptr; }
	std::string getShaderCodeUniforms();
	std::string getOutputShaderCode(int ouputId) override;
	void swapBRDF(const char* filename);
	void swapSpecularMap(const std::string& filename);
	void swapDiffuseMap(const std::string& filename);
	void draw();
private:
	MaterialPBR* material;
	ImGui::FileBrowser fileDialog;
	enum TextureSelection {
		None,
		BrdfLUT,
		SpecularMap,
		DiffuseMap
	} textureSelectionMode = None;
};