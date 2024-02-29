#include "outputnodePhong.h"
#include <glm/gtc/type_ptr.hpp>

OutputNodePhong::OutputNodePhong(Material* material) : OutputNode(material)
{
	ImNodes::SetNodeGridSpacePos(getId(), ImVec2(400, 0));
}

void OutputNodePhong::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 255, 80, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 255, 0, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 0, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Output node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(0);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("kd", glm::value_ptr(getMaterial()->material.kd));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(1);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("ka", glm::value_ptr(getMaterial()->material.ka));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(2);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("ks", glm::value_ptr(getMaterial()->material.ks));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(3);
	ImGui::SetNextItemWidth(180);
	ImGui::SliderFloat("shininess", &(getMaterial()->material.shininess), 0.0f, 100.0f, "%.2f");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(4);
	ImGui::Text("Normal");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
