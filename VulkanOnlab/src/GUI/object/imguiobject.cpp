#include "GUI/object/imguiobject.h"
#include <glm/gtc/type_ptr.hpp>
#include "vulkan/scene.h"
#include "GUI/mainmenu.h"
#include "GUI/nodes/nodeeditor.h"
#include "vulkan/material/materialstore.h"

void ImGuiObject::draw(RenderNode* node)
{
	if (ImGui::TreeNode((node->name + "##" + std::to_string(node->id)).c_str())) {
		const char* items[] = { "Translate", "Scale", "Rotate" };
		ImGui::SetNextItemWidth(20);
		if (ImGui::BeginCombo("Add Transformation", "")) {
			bool is_selected = true;
			for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
				if (ImGui::Selectable(items[i], is_selected)) {
					if (items[i] == "Translate") {
						Translation* t = new Translation(glm::vec3(0));
						node->addTransform(t);
					}
					else if (items[i] == "Scale") {
						Scale* t = new Scale(glm::vec3(1));
						node->addTransform(t);
					}
					else if (items[i] == "Rotate") {
						Rotation* t = new Rotation(glm::vec3(0.0f, 1.0f, 0.0f), 0);
						node->addTransform(t);
					}
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::TreeNode("Transformations")) {
			for (Transformation* transformation : node->getTransformations()) {
				if (ImGui::TreeNode((transformation->getName() + " #" + std::to_string(transformation->id) + "##" + std::to_string(node->id)).c_str())) {
					ImGuiTransformation::draw(transformation, node);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		auto object = dynamic_cast<Object*>(node);
		if (object) {
			ImGui::SetNextItemWidth(200);
			if (ImGui::BeginCombo("Material", object->getMaterial()->name.c_str())) {
				for (auto material : MaterialStore::materials) {
					bool is_selected = object->getMaterial()->name == material.first->name;
					if (ImGui::Selectable(material.first->name.c_str(), is_selected)) {
						object->swapMaterial(material.first);
					}
				}
				ImGui::EndCombo();
			}

			auto oldCastShadow = object->getShadowCast();
			auto newCastShadow = oldCastShadow;
			ImGui::Checkbox("Cast Shadow", &newCastShadow);
			if (oldCastShadow != newCastShadow) {
				object->toggleShadowCast();
			}
		}

		ImGui::Separator();
		if (ImGui::Button("Add Node")) {
			addChildNode(node);
		}

		ImGui::SameLine();
		if (ImGui::Button("Add Mesh")) {
			addMesh(node);
		}

		ImGui::SameLine();
		if (ImGui::Button("Delete Node")) {
			deleteNode(node);
		}

		for (auto childNode : node->getChildren()) {
			draw(childNode);
		}

		ImGui::TreePop();
	}
}

void ImGuiObject::addChildNode(RenderNode* node)
{
	auto newNode = new RenderNode();
	newNode->name = "Node: ";
	node->addChild(newNode);
}

void ImGuiObject::addMesh(RenderNode* node)
{
}

void ImGuiObject::deleteNode(RenderNode* node)
{
	if (node->getParentNode() != nullptr) {
		node->getParentNode()->removeSubgraph(node);
	}
}
