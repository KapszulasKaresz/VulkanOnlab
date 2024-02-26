#include "outputnode.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

OutputNode::OutputNode(Material* material) : material(material) 
{
	ImNodes::SetNodeGridSpacePos(nodeId, ImVec2(400, 0));
}

void OutputNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 255, 80, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 255, 0, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 0, 255));
	ImNodes::BeginNode(nodeId);
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("output node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(0);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("kd", glm::value_ptr(material->material.kd));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(1);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("ka", glm::value_ptr(material->material.ka));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(2);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("ks", glm::value_ptr(material->material.ks));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(3);
	ImGui::SetNextItemWidth(180);
	ImGui::SliderFloat("shininess", &(material->material.shininess), 0.0f, 100.0f, "%.2f");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
