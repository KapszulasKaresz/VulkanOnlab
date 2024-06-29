#include "GUI/transform/imguirotation.h"

void ImGuiRotation::draw()
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
}
