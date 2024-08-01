#pragma once
#include "imnodes.h"
#include <glm/glm.hpp>
#include "GUI/nodes/output/outputnode.h"
#include "vulkan/material/materialEnvMap.h"
#include <imfilebrowser.h>
#include <GUI/nodes/cubemappreview.h>

class OutputNodeEnvMap :public OutputNode {
public:
	OutputNodeEnvMap(MaterialEnvMap* material);
	Material* getMaterial() { return nullptr; }
	std::string getShaderCodeUniforms();
	std::string getOutputShaderCode(int ouputId) override;
	std::string getFunctionDefinitions() override;
	int getPreBindedResourceCount() override { return 1; }
	void swapCubeMap(const std::string& filename);
	void draw();
	bool rayTracedShadows = false;
private:
	CubeMapPreview cubeMapPreviewer;
	MaterialEnvMap* material;
	ImGui::FileBrowser fileDialog;
};