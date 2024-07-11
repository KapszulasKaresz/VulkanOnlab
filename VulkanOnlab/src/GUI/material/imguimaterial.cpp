#include "GUI/material/imguimaterial.h"
#include <misc/cpp/imgui_stdlib.h>
#include "vulkan/material/materialstore.h"

void ImGuiMaterial::draw(Material* material, NodeEditor* nodeEditor)
{
	if (ImGui::Button("Node editor")) {
		nodeEditor->open(material->name);
	}

	if (ImGui::Button("Rename")) {
		renameWindow.open(material);
	}

	nodeEditor->draw();

	if (ImGui::Button("Delete")) {
		MaterialStore::removeMaterial(material->id);
	}
}
