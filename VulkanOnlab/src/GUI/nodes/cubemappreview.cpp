#include "GUI/nodes/cubemappreview.h"
#include <imgui.h>
#include "vulkan/texture/Texture2D.h"

void CubeMapPreview::open(Cubemap* cubemap)
{
	display = true;
	this->cubemap = cubemap;
}

void CubeMapPreview::close()
{
	display = false;
}

void CubeMapPreview::draw()
{
	if (display) {
		if (!ImGui::Begin("Cubemap Previewer", &display, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize)) {
			ImGui::End();
		}

		if (cubemap != nullptr){
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 150);
			ImGui::Image((ImTextureID)cubemap->previewTextures[2]->DS, ImVec2(150, 150));

			ImGui::Dummy(ImVec2(0, 0));
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
			ImGui::Image((ImTextureID)cubemap->previewTextures[1]->DS, ImVec2(150, 150));
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
			ImGui::Image((ImTextureID)cubemap->previewTextures[4]->DS, ImVec2(150, 150));
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
			ImGui::Image((ImTextureID)cubemap->previewTextures[0]->DS, ImVec2(150, 150));
			
			ImGui::SameLine();
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 8);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
			ImGui::Image((ImTextureID)cubemap->previewTextures[5]->DS, ImVec2(150, 150));
			
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 150);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 10);
			ImGui::Image((ImTextureID)cubemap->previewTextures[3]->DS, ImVec2(150, 150));
		}

		ImGui::End();
	}
}
