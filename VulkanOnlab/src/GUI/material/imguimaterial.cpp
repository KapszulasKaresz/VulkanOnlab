#include "GUI/material/imguimaterial.h"
#include <imgui.h>
#include "vulkan/material/materialstore.h"

void ImGuiMaterial::draw(Material* material, NodeEditor* nodeEditor)
{
	if (ImGui::Button("Node editor")) {
		nodeEditor->open(material->name);
	}

	nodeEditor->draw();

	if (ImGui::Button("Delete")) {
		MaterialStore::removeMaterial(material->id);
	}
}
