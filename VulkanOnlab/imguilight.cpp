#include "imguilight.h"
#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

void ImGuiLight::draw()
{
	ImGui::SliderFloat("X", &(light->pos.x), -100.0f, 100.0f, "%.2f");
	ImGui::SliderFloat("Y", &(light->pos.y), -100.0f, 100.0f, "%.2f");
	ImGui::SliderFloat("Z", &(light->pos.z), -100.0f, 100.0f, "%.2f");
	ImGui::ColorEdit3("La", glm::value_ptr(light->La));
	ImGui::ColorEdit3("Le", glm::value_ptr(light->Le));
	ImGui::Checkbox("Directional Light", &directional);
	if (directional) {
		light->pos.w = 0.0f;
	}
	else {
		light->pos.w = 1.0f;
	}
}
