#include "imguiobject.h"
#include <glm/gtc/type_ptr.hpp>

void ImGuiObject::draw()
{
	ImGui::Button("Load Texture");
	ImGui::ColorEdit3("kd", glm::value_ptr(object->material->kd));
	ImGui::ColorEdit3("ka", glm::value_ptr(object->material->ka));
	ImGui::ColorEdit3("ks", glm::value_ptr(object->material->ks));
	ImGui::SliderFloat("shininess", &(object->material->shininess), 0.0f, 100.0f, "%.2f");
}
