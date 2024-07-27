#include "GUI/nodes/edit/vecassemblernode.h"
#include <string>
#include <array>

template<size_t S>
std::string VecAssemblerNode<S>::getOutputShaderCode(int ouputId)
{
	std::string ret;
	ret += "vec" + std::to_string(S) + "(";

	for (int i = 0; i < S; i++) {
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

template<size_t S>
void VecAssemblerNode<S>::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(153, 153, 153, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(119, 119, 119, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(85, 85, 85, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(("Vec" + std::to_string(S) + " assembler node").c_str());
	ImNodes::EndNodeTitleBar();

	for (int i = 0; i < S; i++) {
		ImNodes::BeginInputAttribute(getId() * 10 + i);
		ImGui::Text(components[i].c_str());
		ImNodes::EndInputAttribute();
	}

	ImNodes::BeginOutputAttribute(getId() * 10 + 4);
	ImGui::Text(("vec" + std::to_string(S)).c_str());
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
