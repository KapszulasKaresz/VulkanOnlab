#include "GUI/nodes/mixernode.h"
#include <imnodes.h>

MixerNode::MixerNode(int id) : Node(id)
{
}

void MixerNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 172, 223, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(85, 208, 255, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 128, 191, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Mixer node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(getId() * 10 + 0);
	ImNodes::EndOutputAttribute();

	ImNodes::BeginOutputAttribute(getId() * 10 + 1);
	ImGui::SetNextItemWidth(50);
	ImGui::SliderFloat("mix", &mix, 0.0f, 1.0f, "%.2f");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(getId() * 10 + 2);
	ImNodes::EndOutputAttribute();


	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
