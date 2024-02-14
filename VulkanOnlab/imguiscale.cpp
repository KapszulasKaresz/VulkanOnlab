#include "imguiscale.h"

void ImGuiScale::draw()
{
	ImGui::SliderFloat("X", &(scale->scale.x), -10.0f, 10.0f, "%.2f");
	ImGui::SliderFloat("Y", &(scale->scale.y), -10.0f, 10.0f, "%.2f");
	ImGui::SliderFloat("Z", &(scale->scale.z), -10.0f, 10.0f, "%.2f");
}
