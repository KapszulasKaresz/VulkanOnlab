#include "nodeeditor.h"

void NodeEditor::open(std::string& name)
{
	display = true;
	this->name = name;
}

void NodeEditor::close()
{
	display = false;
}

void NodeEditor::draw()
{
	if (display) {
		if (!ImGui::Begin(name.c_str(), &display)) {
			ImGui::End();
		}
		ImNodes::BeginNodeEditor();

		ImNodes::EndNodeEditor();
		ImGui::End();
	}
}
