#include "mainmenu.h"

void MainMenu::draw()
{
	if (!ImGui::Begin("Main widget")) {
		ImGui::End();
		return; 
	}

	if (ImGui::CollapsingHeader("Load")) {
		ImGui::PushItemWidth(ImGui::GetFontSize() * -12);
		ImGui::Button("Add Object");
	}

	if (ImGui::CollapsingHeader("Lights")) {
		ImGui::Button("Add Light");
		int i = 1;
		for (ImGuiLight* light : lights) {
			std::string name = "Light #";
			name.append(std::to_string(i));
			if (ImGui::TreeNode(name.c_str())) {
				light->draw();
				ImGui::TreePop();
			}
			i++;
		}
	}

	if (ImGui::CollapsingHeader("Objects")) {
		for (ImGuiObject* object : objects) {
			if (ImGui::TreeNode(object->name.c_str())) {
				object->draw();
				ImGui::TreePop();
			}
		}
	}


	ImGui::End();
}

MainMenu::~MainMenu()
{
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}

	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}
}
