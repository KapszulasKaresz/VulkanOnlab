#include "GUI/nodes/input/floatpickernode.h"
#include <imnodes.h>

std::string FloatNode::getOutputShaderCode(int ouputId)
{
    std::string ret = std::to_string(number);
    return ret;
}

void FloatNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(125, 13, 195, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(185, 92, 244, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(87, 9, 135, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Float node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginOutputAttribute(getId() * 10 + 0);
	ImGui::SetNextItemWidth(30);
	ImGui::InputFloat("number", &number);
	ImNodes::EndOutputAttribute();


	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
