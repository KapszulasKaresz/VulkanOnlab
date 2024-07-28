#pragma once
#include "GUI/nodes/node.h"
#include <string>
#include <vector>
#include "imgui.h"
#include "imfilebrowser.h"


struct CustomCodeNode : Node {
	CustomCodeNode(int id);

	std::string getOutputShaderCode(int ouputId) override;
	std::string getFunctionDefinitions() override;

	void draw();
private:
	std::string code;

	std::string header = "";

	ImGui::FileBrowser filebrowser;
	std::vector<int> inputAttributes;

	void loadCode(const std::filesystem::path& path);
	void updateHeader();
	bool hasCodeChanged = false;

	void splitStr(std::string const& str, const char delim,
		std::vector <std::string>& out);

	int currentInputID = 1;
};