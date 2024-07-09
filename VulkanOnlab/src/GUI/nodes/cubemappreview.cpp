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
		if (!ImGui::Begin("Cubemap Previewer", &display, ImGuiWindowFlags_NoCollapse)) {
			ImGui::End();
		}

		if (cubemap != nullptr) {
			ImGui::Image((ImTextureID)cubemap->previewTextures[2]->DS, ImVec2(150, 150));
			ImGui::Image((ImTextureID)cubemap->previewTextures[5]->DS, ImVec2(150, 150));
			ImGui::SameLine();
			ImGui::Image((ImTextureID)cubemap->previewTextures[0]->DS, ImVec2(150, 150));
			ImGui::SameLine();
			ImGui::Image((ImTextureID)cubemap->previewTextures[4]->DS, ImVec2(150, 150));
			ImGui::SameLine();
			ImGui::Image((ImTextureID)cubemap->previewTextures[1]->DS, ImVec2(150, 150));
			ImGui::Image((ImTextureID)cubemap->previewTextures[3]->DS, ImVec2(150, 150));
		}

		ImGui::End();
	}
}
