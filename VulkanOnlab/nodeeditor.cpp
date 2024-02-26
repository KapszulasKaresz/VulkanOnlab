#include "nodeeditor.h"
#include <iostream>

NodeEditor::NodeEditor(Material* material) : material(material), sharedGraphInfo(material->graphInfo) { 
	outputNode = new OutputNode(material); 

}

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
		if (!ImGui::Begin(name.c_str(), &display, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse)) {
			ImGui::End();
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("Add")) {
				const char* names[] = {
					"Texture node"
				};

				for (int i = 0; i < 1; i++)
				{
					if (ImGui::MenuItem(names[i]))
						if (names[i] == "Texture node") {
							nodes.push_back(new TextureNode(nodeId++, sharedGraphInfo));
						}
				}

				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Apply")) {
				material->swapAlbedoTexture(nodes[0]->selectedTexturePath.c_str());
			}
			ImGui::EndMenuBar();
		}

		ImNodes::BeginNodeEditor();
		
		for (Node* node : nodes) {
			node->draw();
		}

		outputNode->draw();

		ImNodes::EndNodeEditor(); 
		ImGui::End();
	}
}

NodeEditor::~NodeEditor()
{
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	delete outputNode;
}
