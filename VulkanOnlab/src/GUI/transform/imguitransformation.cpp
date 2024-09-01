#include "GUI/transform/imguitransformation.h"
#include "GUI/object/imguiobject.h"
#include "imGuIZMOquat.h"


void ImGuiTransformation::drawDelete(Transformation* transformation, RenderNode* object)
{
	if (ImGui::Button("Delete")) {
		object->removeTransform(transformation);
	}
}



void ImGuiTransformation::draw(Transformation* transformation, RenderNode* object)
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

void ImGuiTransformation::draw(Translation* translation, RenderNode* object)
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

void ImGuiTransformation::draw(Scale* scale, RenderNode* object)
{
	ImGui::SliderFloat("X", &(scale->scale.x), -10.0f, 10.0f, "%.2f");
	ImGui::SliderFloat("Y", &(scale->scale.y), -10.0f, 10.0f, "%.2f");
	ImGui::SliderFloat("Z", &(scale->scale.z), -10.0f, 10.0f, "%.2f");
	drawDelete(scale, object);
}

void ImGuiTransformation::draw(Rotation* rotation, RenderNode* object)
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

void ShowMat4Control(glm::mat4& mat) {
	float totalWidth = ImGui::GetContentRegionAvail().x;
	float cellWidth = totalWidth / 4.0f;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			char cellLabel[16];
			snprintf(cellLabel, sizeof(cellLabel), "##%d%d", row, col);

			ImGui::PushItemWidth(cellWidth - ImGui::GetStyle().ItemSpacing.x);

			ImGui::PushID(cellLabel);
			ImGui::InputFloat(cellLabel, &mat[row][col], 0.0f, 0.0f, "%.3f");
			ImGui::PopID();

			ImGui::PopItemWidth();

			if (col < 3)
				ImGui::SameLine();
		}
	}
}

void ImGuiTransformation::draw(MatrixTransform* matrixTransform, RenderNode* object)
{
	ShowMat4Control(matrixTransform->transformMatrix);
	drawDelete(matrixTransform, object);
}
