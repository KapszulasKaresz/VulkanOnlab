#include "GUI/nodes/outputnodePBR.h"
#include <glm/gtc/type_ptr.hpp>

OutputNodePBR::OutputNodePBR(MaterialPBR* material) : OutputNode(), material(material)
{
	ImNodes::SetNodeGridSpacePos(getId(), ImVec2(400, 0));
}

std::string OutputNodePBR::getShaderCodeUniforms()
{
	return std::string();
}

std::string OutputNodePBR::getOutputShaderCode(int ouputId)
{
	return std::string();
}

void OutputNodePBR::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 255, 80, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 255, 0, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 0, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Output node PBR");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(0);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("Albedo color", glm::value_ptr(material->material.albedo));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(1);
	ImGui::SetNextItemWidth(180);
	ImGui::SliderFloat("Metallicness", &(material->material.metallic), 0.0f, 1.0f, "%.2f");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(2);
	ImGui::SetNextItemWidth(180);
	ImGui::SliderFloat("Roughness", &(material->material.roughness), 0.0f, 1.0f, "%.2f");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(3);
	ImGui::Text("Normal");
	ImGui::SetNextItemWidth(180);
	
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
