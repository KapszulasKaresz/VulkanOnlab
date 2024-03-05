#include "mathnode.h"
#include "imnodes.h"

MathNode::MathNode(int id) : Node(id)
{
}

void MathNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 172, 223, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(85, 208, 255, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 128, 191, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Math node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginOutputAttribute(getId() * 10 + 3);
	ImGui::Text("Value");
	ImNodes::EndOutputAttribute();
	const char* items[] = { "Add", "Subtract", "Multipy", "Divide"};
	
	ImGui::SetNextItemWidth(100);
	if (ImGui::BeginCombo("", items[selected])) {
			for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
			if(ImGui::Selectable(items[i])){
				if (items[i] == "Add") {
					selected = 0;
				} else if(items[i] == "Subtract") {
					selected = 1;
				} else if (items[i] == "Multipy") {
					selected = 2;
				} else if (items[i] == "Divide") {
					selected = 3;
				}
			}
		}
		ImGui::EndCombo();
	}
		

	ImNodes::BeginInputAttribute(getId() * 10 + 0);
	ImGui::Text("Value a");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(getId() * 10 + 1);
	ImGui::Text("Value b");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

std::string MathNode::getOperator()
{
	std::string ret;

	switch (selected)
	{
	case 0:
		ret = " + "; break;
	case 1: 
		ret = " - "; break;
	case 2:
		ret = " * "; break;
	case 3:
		ret = " / "; break;
	default:
		ret = " + "; break;
	}

	return ret;
}
