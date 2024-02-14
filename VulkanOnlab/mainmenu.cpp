#include "mainmenu.h"
#include "scene.h"

void MainMenu::draw()
{
	if (!ImGui::Begin("Main widget")) {
		ImGui::End();
		return; 
	}

	if (ImGui::CollapsingHeader("Lights")) {
		if (ImGui::Button("Add Light")) {
			scene->addLight();
		}
		int i = 1;
		for (ImGuiLight* light : lights) {
			std::string name = "Light #";
			name.append(std::to_string(light->id));
			if (ImGui::TreeNode(name.c_str())) {
				light->draw();
				ImGui::TreePop();
			}
			i++;
		}
	}

	if (ImGui::CollapsingHeader("Objects")) {
		ImGui::Button("Add Object");
		for (ImGuiObject* object : objects) {
			if (ImGui::TreeNode(object->name.c_str())) {
				object->draw();
				ImGui::TreePop();
			}
		}
	}


	ImGui::End();
}

void MainMenu::addObject(ImGuiObject* object)
{
	objects.push_back(object);
}

void MainMenu::addLight(ImGuiLight* light)
{
	lights.push_back(light); 
}

void MainMenu::removeLight(ImGuiLight* light)
{
	for (int i = 0; i < lights.size(); i++) {
		if (*(lights[i]) == *light) {
			lights.erase(lights.begin() + i);
			break;
		}
	}
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
