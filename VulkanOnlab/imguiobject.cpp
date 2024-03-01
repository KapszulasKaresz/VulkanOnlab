#include "imguiobject.h"
#include <glm/gtc/type_ptr.hpp>
#include "scene.h"
#include "mainmenu.h"
#include "nodeeditor.h"

int ImGuiObject::rollingId = 0;

ImGuiObject::ImGuiObject(Object* object, const char* name, Scene* scene, MainMenu* mainMenu)
	: object(object), name(name), scene(scene), mainMenu(mainMenu), id(rollingId++) 
{
	nodeEditor = new NodeEditor(object->material, object);
}

void ImGuiObject::remove(ImGuiTransformation* transformation)
{
	Transformation* t = transformation->getTransform();

	for (int i = 0; i < object->transformations.size(); i++) {
		if (*t == *(object->transformations[i])) {
			delete object->transformations[i];
			object->transformations.erase(object->transformations.begin() + i);
		}
	}

	for (int i = 0; i < transformations.size(); i++) {
		if (*(transformations[i]) == *transformation) {
			delete transformation;
			transformations.erase(transformations.begin() + i);
		}
	}
}

void ImGuiObject::draw()
{
	if (ImGui::Button("Material Editor")) {
		std::string editorName ="node editor " + name;
		nodeEditor->open(editorName);
	}
	
	const char* items[] = { "Translate", "Scale", "Rotate" };
	ImGui::SetNextItemWidth(20);
	if (ImGui::BeginCombo("Add Transformation", "")) {
		bool is_selected =true;
		for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
			if (ImGui::Selectable(items[i], is_selected)) {
				if (items[i] == "Translate") {
					Translation* t = new Translation(glm::vec3(0));
					object->transformations.push_back(t);
					ImGuiTranslation* imguit = new ImGuiTranslation(t, this);
					transformations.push_back(imguit);
				} else if (items[i] == "Scale") {
					Scale* t = new Scale(glm::vec3(1));
					object->transformations.push_back(t);
					ImGuiScale* imguit = new ImGuiScale(t, this);
					transformations.push_back(imguit);
				}else if (items[i] == "Rotate") {
					Rotation* t = new Rotation(glm::vec3(0.0f,1.0f,0.0f), 0);
					object->transformations.push_back(t);
					ImGuiRotation* imguit = new ImGuiRotation(t, this);
					transformations.push_back(imguit);
				}
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::TreeNode("Transformations")) {
		for (ImGuiTransformation* transformation : transformations) {
			if (ImGui::TreeNode((transformation->name + " #" + std::to_string(transformation->id)).c_str())) {
				transformation->draw();
				transformation->drawDelete();
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	nodeEditor->draw();

	if (ImGui::Button("Delete")) {
		scene->removeObject(object);
		mainMenu->removeObject(this);
	}
}

ImGuiObject::~ImGuiObject()
{
	for (int i = 0; i < transformations.size(); i++) {
		delete transformations[i];
	}
	if (nodeEditor != nullptr) {
		delete nodeEditor;
	}
}