#include "GUI/transform/imguitransformation.h"
#include "GUI/object/imguiobject.h"
#include "imGuIZMOquat.h"


void ImGuiTransformation::drawDelete(Transformation* transformation, ImGuiObject* object)
{
	if (ImGui::Button("Delete")) {
		object->remove(transformation);
	}
}



void ImGuiTransformation::draw(Transformation* transformation, ImGuiObject* object)
{
	if (dynamic_cast<Translation*>(transformation) != nullptr) {
		draw(dynamic_cast<Translation*>(transformation), object);
		return;
	}
	if (dynamic_cast<Rotation*>(transformation) != nullptr) {
		draw(dynamic_cast<Rotation*>(transformation), object);
		return;
	}
	if (dynamic_cast<Scale*>(transformation) != nullptr) {
		draw(dynamic_cast<Scale*>(transformation), object);
		return;
	}
	if (dynamic_cast<MatrixTransform*>(transformation) != nullptr) {
		draw(dynamic_cast<MatrixTransform*>(transformation), object);
		return;
	}
}

void ImGuiTransformation::draw(Translation* translation, ImGuiObject* object)
{
	ImGui::SetNextItemWidth(50);
	ImGui::InputFloat("X", &(translation->translation.x));
	ImGui::SetNextItemWidth(50);
	ImGui::SameLine();
	ImGui::InputFloat("Y", &(translation->translation.y));
	ImGui::SetNextItemWidth(50);
	ImGui::SameLine();
	ImGui::InputFloat("Z", &(translation->translation.z));
	drawDelete(translation, object);
}

void ImGuiTransformation::draw(Scale* scale, ImGuiObject* object)
{
	ImGui::SliderFloat("X", &(scale->scale.x), -10.0f, 10.0f, "%.2f");
	ImGui::SliderFloat("Y", &(scale->scale.y), -10.0f, 10.0f, "%.2f");
	ImGui::SliderFloat("Z", &(scale->scale.z), -10.0f, 10.0f, "%.2f");
	drawDelete(scale, object);
}

void ImGuiTransformation::draw(Rotation* rotation, ImGuiObject* object)
{
	ImGui::gizmo3D("Direction", rotation->rotationAxis, 100, imguiGizmo::modeDirection);
	ImGui::SetNextItemWidth(40);
	ImGui::InputFloat("X", &(rotation->rotationAxis.x));
	ImGui::SetNextItemWidth(40);
	ImGui::SameLine();
	ImGui::InputFloat("Y", &(rotation->rotationAxis.y));
	ImGui::SetNextItemWidth(40);
	ImGui::SameLine();
	ImGui::InputFloat("Z", &(rotation->rotationAxis.z));
	ImGui::SliderAngle("Amount", &(rotation->amount));
	drawDelete(rotation, object);
}

void ImGuiTransformation::draw(MatrixTransform* matrixTransform, ImGuiObject* object)
{
}
