#include "GUI/nodes/outputnodeEnvMap.h"

OutputNodeEnvMap::OutputNodeEnvMap(MaterialEnvMap* material) : OutputNode(), material(material)
{
	ImNodes::SetNodeGridSpacePos(getId(), ImVec2(400, 0));
}

std::string OutputNodeEnvMap::getShaderCodeUniforms()
{
	return std::string();
}

std::string OutputNodeEnvMap::getOutputShaderCode(int ouputId)
{
	return std::string();
}

std::string OutputNodeEnvMap::getFunctionDefinitions()
{
	return std::string();
}

void OutputNodeEnvMap::swapCubeMap(const std::string& filename)
{
	std::string newFileName = filename.substr(0, filename.size() - 8);
	material->swapCubeMap(newFileName.c_str());
}

void OutputNodeEnvMap::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 255, 80, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 255, 0, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 0, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Output node Env Map");
	ImNodes::EndNodeTitleBar();


	if (fileDialog.HasSelected())
	{
		swapCubeMap(fileDialog.GetSelected().string());
		fileDialog.ClearSelected();
	}

	ImGui::Text("Env map");

	if (ImGui::Button("Select Env map")) {
		fileDialog.SetTitle("Pick an image");
		fileDialog.SetTypeFilters({ ".hdr" });
		fileDialog.Open();
	}

	if (ImGui::Button("Preview Env map")) {
		cubeMapPreviewer.open(material->cubeMap);
	}

	fileDialog.Display();
	cubeMapPreviewer.draw();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
