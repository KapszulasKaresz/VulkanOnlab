#include "GUI/nodes/vec4disassemblernode.h"
#include "imnodes.h"
#include <string>
#include <array>

std::string Vec4DisassemblerNode::getOutputShaderCode(int ouputId)
{
	std::string ret = inputs[4].first != NULL ? inputs[4].first->getOutputShaderCode(inputs[4].second) : "vec4(0)";


	switch (ouputId)
	{
	case 0: ret += ".x";  break;
	case 1: ret += ".y"; break;
	case 2: ret += ".z"; break;
	case 3: ret += ".w"; break;
	default: break;
	}

	return ret;
}

void Vec4DisassemblerNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(153, 153, 153, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(119, 119, 119, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(85, 85, 85, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Vec4 disassembler node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(getId() * 10 + 4);
	ImGui::Text("vec4");
	ImNodes::EndInputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 0);
	ImGui::Text("x");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 1);
	ImGui::Text("y");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 2);
	ImGui::Text("z");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 3);
	ImGui::Text("w");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
