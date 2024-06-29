#include "GUI/nodes/vec4assemblernode.h"
#include "imnodes.h"
#include <string>
#include <array>

std::string Vec4AssemblerNode::getOutputShaderCode(int ouputId)
{
	std::string ret;
	ret += "vec4(";

	for (int i = 0; i < 4; i++) {
		if (inputs[i].first != nullptr) {
			if (i != 0) {
				ret += ", ";
			}
			ret += inputs[i].first->getOutputShaderCode(inputs[i].second);
		}
	}

	ret += ")";

    return ret;
}

void Vec4AssemblerNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(153, 153, 153, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(119, 119, 119, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(85, 85, 85, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Vec4 assembler node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(getId() * 10 + 0);
	ImGui::Text("x");
	ImNodes::EndInputAttribute();

	ImNodes::BeginInputAttribute(getId() * 10 + 1);
	ImGui::Text("y");
	ImNodes::EndInputAttribute();

	ImNodes::BeginInputAttribute(getId() * 10 + 2);
	ImGui::Text("z");
	ImNodes::EndInputAttribute();

	ImNodes::BeginInputAttribute(getId() * 10 + 3);
	ImGui::Text("w");
	ImNodes::EndInputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 4);
	ImGui::Text("vec4");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
