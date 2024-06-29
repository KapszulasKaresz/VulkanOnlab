#include "GUI/nodes/colorconstnode.h"
#include <imnodes.h>
#include <glm/gtc/type_ptr.hpp>

std::string ColorConstNode::getOutputShaderCode(int ouputId)
{
	std::string ret;

	ret += "vec4(" + std::to_string(color.x) + ", " + std::to_string(color.y) 
		+ ", " + std::to_string(color.z) + ", " + std::to_string(color.w) + ")";

	return ret;
}

void ColorConstNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(125, 13, 195, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(185, 92, 244, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(87, 9, 135, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Constant Color Picker node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginOutputAttribute(getId() * 10 + 0);
	ImGui::SetNextItemWidth(100);
	ImGui::ColorPicker3("ColorPicker", glm::value_ptr(color));
	ImNodes::EndOutputAttribute();


	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
