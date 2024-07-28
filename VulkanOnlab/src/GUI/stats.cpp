#include "GUI/stats.h"
#include <imgui.h>
#include <string>
#include "vulkan/application.h"

void StatWindow::open()
{
	display = true;
}

void StatWindow::draw()
{
	if (display) {
		if (!ImGui::Begin("Stats", &display, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
			ImGui::End();
		}

		ImGui::Text((std::string("FPS: ") + std::to_string(1.0 / Application::deltaT)).c_str());
		ImGui::Text((std::string("Frame time: ") + std::to_string(Application::deltaT) + "s").c_str());

		ImGui::End();
	}
}
