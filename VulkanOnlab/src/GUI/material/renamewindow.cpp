#include "GUI/material/renamewindow.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

void RenameWindow::draw()
{
	if (display) {
		ImGui::SetNextWindowSize(ImVec2(150,80));
		if (!ImGui::Begin("Rename", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
			ImGui::End();
		}

		ImGui::InputText(" ", &currentName);

		if (ImGui::Button("OK")) {
			display = false;
			material->name = currentName;
		}

		ImGui::SameLine();
		if (ImGui::Button("Cancel")) {
			display = false;
		}

		ImGui::End();
	}
}

void RenameWindow::open(Material* material)
{
	display = true;
	currentName = material->name;
	this->material = material;
}
