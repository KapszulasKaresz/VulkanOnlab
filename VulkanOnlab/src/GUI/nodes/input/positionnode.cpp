#include "GUI/nodes/input/positionnode.h"
#include "imnodes.h"

std::string PositionNode::getOutputShaderCode(int ouputId)
{
	std::string ret;

	switch (ouputId)
	{
	case 0: ret += "wPos";  break;
	case 1: ret += "vec4(N, 1.0)"; break;
	case 2: ret += "texCoord"; break;
	default: break;
	}

	return ret;
}

void PositionNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(125, 13, 195, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(185, 92, 244, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(87, 9, 135, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::Text("Position node");
	ImNodes::EndNodeTitleBar();


	ImNodes::BeginOutputAttribute(getId() * 10 + 0);
	ImGui::Text("Position");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 1);
	ImGui::Text("Normal");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 2);
	ImGui::Text("Tex Coord");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();


	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
