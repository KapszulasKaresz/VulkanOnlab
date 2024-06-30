#include "GUI/nodes/vecdisassemblernode.h"
#include "imnodes.h"
#include <string>
#include <array>

template<size_t S>
std::string VecDisassemblerNode<S>::getOutputShaderCode(int ouputId)
{
	std::string ret = inputs[4].first != NULL ? inputs[4].first->getOutputShaderCode(inputs[4].second) : "vec" + std::to_string(S) + "(0)";


	switch (ouputId)
	{
	case 0: ret += ".x"; break;
	case 1: ret += ".y"; break;
	case 2: ret += ".z"; break;
	case 3: ret += ".w"; break;
	default: break;
	}

	return ret;
}

template<size_t S>
void VecDisassemblerNode<S>::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(153, 153, 153, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(119, 119, 119, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(85, 85, 85, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted(("Vec" + std::to_string(S) + " disassembler node").c_str());
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(getId() * 10 + 4);
	ImGui::Text(("vec" + std::to_string(S)).c_str());
	ImNodes::EndInputAttribute();

	for (int i = 0; i < S; i++) {
		ImNodes::BeginOutputAttribute(getId() * 10 + i);
		ImGui::Text(components[i]).c_str());
		ImNodes::EndOutputAttribute();
	}

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
