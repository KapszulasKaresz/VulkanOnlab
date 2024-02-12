#include "imguilight.h"
#include "scene.h"
#include "mainmenu.h"

int ImGuiLight::rollingId =0;

ImGuiLight::ImGuiLight(Light* light, Scene* scene, MainMenu* mainmenu) : light(light), scene(scene), mainmenu(mainmenu) 
{
	id = rollingId;
	ImGuiLight::rollingId++;
}

bool ImGuiLight::operator==(ImGuiLight& other)
{
	return id == other.id;
}

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

	if (ImGui::Button("Delete")) {
		mainmenu->removeLight(this);
		scene->deleteLight(light);
	}
}
