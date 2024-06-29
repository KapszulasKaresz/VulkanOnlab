#include "GUI/transform/imguitranslation.h"

void ImGuiTranslation::draw()
{
	ImGui::SetNextItemWidth(50);
	ImGui::InputFloat("X", &(translation->translation.x));
	ImGui::SetNextItemWidth(50);
	ImGui::SameLine();
	ImGui::InputFloat("Y", &(translation->translation.y));
	ImGui::SetNextItemWidth(50);
	ImGui::SameLine();
	ImGui::InputFloat("Z", &(translation->translation.z));
}
