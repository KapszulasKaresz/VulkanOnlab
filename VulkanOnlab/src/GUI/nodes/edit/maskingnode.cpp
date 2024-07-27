#include "GUI/nodes/edit/maskingnode.h"
#include "imnodes.h"

MaskingNode::MaskingNode(int id) : Node(id)
{
}

std::string MaskingNode::getOutputShaderCode(int ouputId)
{
	std::string ret, first, second;

	if (inputs[0].first == nullptr) {
		first = "vec4(0)";
	}
	else {
		first = inputs[0].first->getOutputShaderCode(inputs[0].second);
	}

	if (inputs[1].first == nullptr) {
		second = "vec4(0)";
	}
	else {
		second = inputs[1].first->getOutputShaderCode(inputs[1].second);
	}

	ret += "(" + first + " * " + second + ")";
	 
	return ret;
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
