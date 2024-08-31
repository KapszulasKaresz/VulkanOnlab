#pragma once
#include "imnodes.h"
#include <glm/glm.hpp>
#include "GUI/nodes/output/outputnode.h"
#include "vulkan/material/materialPBR.h"
#include <imfilebrowser.h>
#include <GUI/nodes/cubemappreview.h>

class OutputNodePBR :public OutputNode{
public:
	OutputNodePBR(MaterialPBR* material);
	Material* getMaterial() { return nullptr; }
	std::string getShaderCodeUniforms();
	std::string getOutputShaderCode(int ouputId) override;
	std::string getFunctionDefinitions() override;
	int getPreBindedResourceCount() override { return 4; }
	void swapBRDF(const char* filename);
	void swapSpecularMap(const std::string& filename);
	void swapDiffuseMap(const std::string& filename);
	void draw();
private:
	CubeMapPreview cubeMapPreviewer;
	MaterialPBR* material;
	ImGui::FileBrowser fileDialog;
	enum TextureSelection {
		None,
		BrdfLUT,
		SpecularMap,
		DiffuseMap
	} textureSelectionMode = None;
};