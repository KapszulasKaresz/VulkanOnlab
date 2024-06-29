#include "GUI/nodes/checkeredtexturenode.h"
#include "imnodes.h"
#include "imgui_impl_vulkan.h"
#include <glm/gtc/type_ptr.hpp>


CheckeredTextureNode::CheckeredTextureNode(int id, SharedGraphicsInfo graphInfo) : TextureNode(id, graphInfo)
{
	color1 = glm::vec3(0.0f);
	color2 = glm::vec3(1.0f);
}


void CheckeredTextureNode::draw()
{
	if (generate) {
		if (getTexture() == nullptr) {
			createTexture();
		}
		else {
			vkDeviceWaitIdle(*(graphInfo.device));
			ImGui_ImplVulkan_RemoveTexture(getTexture()->DS);
			if (!hasBeenAssigned) {
				getTexture()->reset();
			}
			else {
				createTexture();
			}
			hasBeenAssigned = false;
		}

		getTexture()->generateCheckered(color1, color2, scale);
		getTexture()->DS = ImGui_ImplVulkan_AddTexture(getTexture()->getTextureSampler(), getTexture()->getTextureImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		generate = false;
	}

	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(125, 13, 195, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(185, 92, 244, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(87, 9, 135, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::Text("Checkered Texture node");
	ImNodes::EndNodeTitleBar();

	if (getTexture() != nullptr) {
		ImGui::Image((ImTextureID)getTexture()->DS, ImVec2(150, 150));
	}

	if (ImGui::Button("Generate")) {
		generate = true;
	}
	ImGui::SetNextItemWidth(100); 
	ImGui::ColorEdit3("Color1", glm::value_ptr(color1));
	ImGui::SetNextItemWidth(100);
	ImGui::ColorEdit3("Color2", glm::value_ptr(color2));
	ImGui::SetNextItemWidth(150);
	ImGui::SliderFloat("scale", &scale, 1.0f, 30.0f, "%.2f");

	ImNodes::BeginOutputAttribute(getId() * 10 + 0);
	ImGui::Text("Texture data");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();

	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

