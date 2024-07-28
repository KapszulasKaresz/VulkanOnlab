#include "GUI/nodes/edit/customcodenode.h"
#include "imnodes.h"
#include <misc/cpp/imgui_stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>

CustomCodeNode::CustomCodeNode(int id) : Node(id)
{
}

std::string CustomCodeNode::getOutputShaderCode(int ouputId)
{
	std::string ret;

	ret = header;
	ret += "(";
	for (int i = 0; i < inputAttributes.size(); i++) {
		if (inputs[inputAttributes[i]].first == nullptr) {
			ret += "invalid input";
		}
		else
		{
			ret += inputs[inputAttributes[i]].first->getOutputShaderCode(inputs[inputAttributes[i]].second);
		}
		if (i < inputAttributes.size() - 1) {
			ret += ",";
		}
	}
	ret += ")";

	return ret;
}

std::string CustomCodeNode::getFunctionDefinitions()
{
	return code;
}

void CustomCodeNode::draw()
{
	if (header == "") {
		ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(173, 0, 0, 255));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(200, 0, 0, 255));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(255, 0, 0, 255));
	}
	else {
		ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 172, 223, 255));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(85, 208, 255, 255));
		ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 128, 191, 255));
	}
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Custom Code node");
	if (header == "") {
		ImGui::TextUnformatted("The header is incorrect for the function");
	}
	ImNodes::EndNodeTitleBar();

	std::string oldCode = code;

	ImGui::SetNextItemWidth(300);
	ImGui::InputTextMultiline(" ", &code, ImVec2(0,0), 0);

	hasCodeChanged = !(oldCode == code);

	if (ImGui::Button("Open code")) {
		filebrowser.Open();
	}

	filebrowser.Display();

	if (filebrowser.HasSelected()) {
		loadCode(filebrowser.GetSelected());
		filebrowser.ClearSelected();
	}

	for (int i = 0; i < inputAttributes.size(); i++) {
		ImNodes::BeginInputAttribute(getId() * 10 + inputAttributes[i]);
		ImGui::Text(("Input #" + std::to_string(inputAttributes[i])).c_str());
		ImNodes::EndInputAttribute();
	}

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 150);
	if (ImGui::Button("+ Input")) {
		inputAttributes.push_back(currentInputID++);
	}

	updateHeader();

	ImNodes::BeginOutputAttribute(getId() * 10 + 0);
	ImGui::Text("Output");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

void CustomCodeNode::loadCode(const std::filesystem::path& path)
{
	std::ifstream file(path);

	std::stringstream buffer;
	buffer << file.rdbuf();

	code = buffer.str();

	file.close();

	hasCodeChanged = true;
}

void CustomCodeNode::updateHeader()
{
	if (hasCodeChanged) {
		hasCodeChanged = false;
		std::vector<std::string> codeTokens;
		splitStr(code, ' ', codeTokens);
		if (codeTokens.size() > 1) {
			std::vector<std::string> headerTokens;
			splitStr(codeTokens[1], '(', headerTokens);
			header = headerTokens[0];
		}
		else {
			header = "";
		}
	}
}

void CustomCodeNode::splitStr(std::string const& str, const char delim, std::vector<std::string>& out)
{
	std::stringstream s(str); 

	std::string s2; 
	while (std::getline(s, s2, delim))
	{
		out.push_back(s2);
	}
}
