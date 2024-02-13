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
	dir = light->pos;
	if (directional) {
		ImGui::gizmo3D("Direction", dir, 100, imguiGizmo::modeDirection);
		light->pos =glm::vec4(dir, 0.0f);
	}
	else {
		ImGui::gizmo3D("Position", dir, updir, 100, imguiGizmo::modePanDolly);
		light->pos = glm::vec4(dir, 1.0f);
	}

	ImGui::ColorEdit3("La", glm::value_ptr(light->La));
	ImGui::ColorEdit3("Le", glm::value_ptr(light->Le));
	ImGui::Checkbox("Directional Light", &directional);
	

	if (ImGui::Button("Delete")) {
		mainmenu->removeLight(this);
		scene->deleteLight(light);
	}
}
