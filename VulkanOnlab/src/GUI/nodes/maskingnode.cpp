#include "GUI/nodes/maskingnode.h"
#include "imnodes.h"

MaskingNode::MaskingNode(int id) : Node(id)
{
}

void MaskingNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 172, 223, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(85, 208, 255, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 128, 191, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Masking node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(getId() * 10 + 0);
	ImGui::Text("Input");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(getId() * 10 + 1);
	ImGui::Text("Mask");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 2);
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
