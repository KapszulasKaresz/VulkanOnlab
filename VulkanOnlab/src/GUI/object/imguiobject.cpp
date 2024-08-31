#include "GUI/object/imguiobject.h"
#include <glm/gtc/type_ptr.hpp>
#include "vulkan/scene.h"
#include "GUI/mainmenu.h"
#include "GUI/nodes/nodeeditor.h"
#include "vulkan/material/materialstore.h"

int ImGuiObject::rollingId = 0;

ImGuiObject::ImGuiObject(Object* object, const char* name, Scene* scene, MainMenu* mainMenu)
	: object(object), name(name), scene(scene), mainMenu(mainMenu), id(rollingId++) 
{
}

void ImGuiObject::remove(Transformation* transformation)
{
	object->removeTransform(transformation);
}

void ImGuiObject::draw()
{
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
			
	const char* items[] = { "Translate", "Scale", "Rotate" };
	ImGui::SetNextItemWidth(20);
	if (ImGui::BeginCombo("Add Transformation", "")) {
		bool is_selected =true;
		for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
			if (ImGui::Selectable(items[i], is_selected)) {
				if (items[i] == "Translate") {
					Translation* t = new Translation(glm::vec3(0));
					object->addTransform(t);
				} else if (items[i] == "Scale") {
					Scale* t = new Scale(glm::vec3(1));
					object->addTransform(t);
				}else if (items[i] == "Rotate") {
					Rotation* t = new Rotation(glm::vec3(0.0f,1.0f,0.0f), 0);
					object->addTransform(t);
				}
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::TreeNode("Transformations")) {
		for (Transformation* transformation : object->getTransformations()) {
			if (ImGui::TreeNode((transformation->getName() + " #" + std::to_string(transformation->id) + "##" + std::to_string(id)).c_str())) {
				ImGuiTransformation::draw(transformation, this);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::Button("Delete")) {
		scene->removeObject(object);
		mainMenu->removeObject(this);
	}
}

ImGuiObject::~ImGuiObject()
{
}
