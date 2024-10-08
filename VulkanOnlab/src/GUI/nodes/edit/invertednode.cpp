#include "GUI/nodes/edit/invertednode.h"
#include "imnodes.h"

InverterNode::InverterNode(int id) : Node(id)
{
}

std::string InverterNode::getOutputShaderCode(int ouputId)
{
	std::string ret, first;

	if (inputs[0].first == nullptr) {
		first = "vec4(0)";
	}
	else {
		first = inputs[0].first->getOutputShaderCode(inputs[0].second);
	}

	ret += "(vec4(1) - " + first + ")";

	return ret;
}

void InverterNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 172, 223, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(85, 208, 255, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 128, 191, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Inverter node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(getId() * 10 + 0);
	ImGui::Text("Input");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 1);
	ImGui::Text("Output");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
