#include "GUI/mainmenu.h"
#include "vulkan/scene.h"
#include "vulkan/material/materialPhong.h"
#include "vulkan/material/materialstore.h"
#include <iostream>
#include <filesystem>


void MainMenu::draw()
{
	if (!ImGui::Begin("Main widget")) {
		ImGui::End();
		return; 
	}

	if (ImGui::Button("Load GLTF scene"))
	{
		fileDialogGLTF.SetPwd(std::filesystem::current_path() / "res");
		fileDialogGLTF.SetTitle("Pick a gltf scene");
		fileDialogGLTF.SetTypeFilters({ ".gltf", ".glb"});
		fileDialogGLTF.Open();
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
		if (ImGui::Button("Add Object")) {
			fileDialogObj.SetPwd(std::filesystem::current_path() / "res");
			fileDialogObj.SetTitle("Pick an object");
			fileDialogObj.SetTypeFilters({ ".obj" });
			fileDialogObj.Open();
		}
		for (ImGuiObject* object : objects) {
			if (ImGui::TreeNode((object->name + "##" + std::to_string(object->id)).c_str())) {
				object->draw();
				ImGui::TreePop();
			}
		}
	}

	if (ImGui::Button("Enable ray-traced shadows"))
	{
		MaterialStore::toggleRaytracedShadows(true);
	}

	ImGui::SameLine();
	if (ImGui::Button("Disable ray-traced shadows"))
	{
		MaterialStore::toggleRaytracedShadows(false);
	}

	if (ImGui::CollapsingHeader("Materials")) {
		if (ImGui::Button("Add Material")) {
			MaterialStore::addMaterial(new MaterialPhong());
		}

		for (auto material : MaterialStore::materials) {
			if (ImGui::TreeNode((material.first->name + "##" + std::to_string(material.first->id)).c_str())) {
				materialDrawer.draw(material.first, material.second);
				ImGui::TreePop();
			}
		}

		materialDrawer.renameWindow.draw();
	}

	if (ImGui::Button("Show stats")) {
		statWindow.open();
	}

	statWindow.draw();

	fileDialogObj.Display();
	fileDialogGLTF.Display();

	if (fileDialogGLTF.HasSelected())
	{
		scene->loadGLTFScene(fileDialogGLTF.GetSelected(), this);
		fileDialogGLTF.ClearSelected();
	}

	if (fileDialogObj.HasSelected())
	{
		scene->addObject(fileDialogObj.GetSelected().string().c_str(), this);
		fileDialogObj.ClearSelected();
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

void MainMenu::removeObject(ImGuiObject* object)
{
	for (int i = 0; i < objects.size(); i++) {
		if (*(objects[i]) == *object) {
			delete objects[i];
			objects.erase(objects.begin() + i);
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
