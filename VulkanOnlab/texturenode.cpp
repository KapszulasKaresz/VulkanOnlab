#include "texturenode.h"
#include "imnodes.h"
#include "imgui_impl_vulkan.h"

TextureNode::TextureNode(int id, SharedGraphicsInfo graphInfo) : graphInfo(graphInfo), Node(id) 
{
}

void TextureNode::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(125, 13, 195, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(185, 92, 244, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(87, 9, 135, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::Text("Texture node");
	ImNodes::EndNodeTitleBar();

	ImGui::TextUnformatted(selectedTexture.c_str());

	if (fileDialog.HasSelected())
	{
		selectedTexture = fileDialog.GetSelected().filename().string();
		selectedTexturePath = fileDialog.GetSelected().string();

		if (texture == nullptr) {
			texture = new Texture(graphInfo);
		}
		else {
			vkDeviceWaitIdle(*(graphInfo.device));
			ImGui_ImplVulkan_RemoveTexture(texture->DS);
			if (!hasBeenAssigned) {
				texture->reset();
			}
			hasBeenAssigned = false;
		}

		texture->load(fileDialog.GetSelected().string().c_str());
		texture->DS = ImGui_ImplVulkan_AddTexture(texture->getTextureSampler(), texture->getTextureImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		fileDialog.ClearSelected();
	}

	if (selectedTexture != " ") {
		ImGui::Image((ImTextureID)texture->DS, ImVec2(150, 150));
	}

	if (ImGui::Button("Select texture")) {
		fileDialog.SetTitle("Pick an image");
		fileDialog.SetTypeFilters({ ".png", ".jpg"});
		fileDialog.Open();
	}

	ImNodes::BeginOutputAttribute(getId() * 10 + 0);
	ImGui::Text("Texture data");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();

	fileDialog.Display();


	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}

TextureNode::~TextureNode()
{
	if (texture != nullptr) {
		if (!hasBeenAssigned) {
			ImGui_ImplVulkan_RemoveTexture(texture->DS);
			texture->reset();
			delete texture;
		}
	}
}
