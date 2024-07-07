#include "GUI/nodes/outputnodePBR.h"
#include <glm/gtc/type_ptr.hpp>
#include "imgui_impl_vulkan.h"
#include "vulkan/texture/texture.h"
#include "vulkan/texture/texture2D.h"
#include "vulkan/texture/Cubemap.h"
#include "vulkan/application.h"

OutputNodePBR::OutputNodePBR(MaterialPBR* material) : OutputNode(), material(material)
{
	ImNodes::SetNodeGridSpacePos(getId(), ImVec2(400, 0));
	material->brdfLUT->DS = ImGui_ImplVulkan_AddTexture(material->brdfLUT->getTextureSampler(), material->brdfLUT->getTextureImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

std::string OutputNodePBR::getShaderCodeUniforms()
{
	return std::string();
}

std::string OutputNodePBR::getOutputShaderCode(int ouputId)
{
	return std::string();
}

void OutputNodePBR::swapBRDF(const char* filename)
{
	material->swapBRDF(filename);
}

void OutputNodePBR::swapSpecularMap(const std::string& filename)
{
	std::string newFileName = filename.substr(0, filename.size() - 8);
	material->swapSpecularMap(newFileName.c_str());
}

void OutputNodePBR::swapDiffuseMap(const std::string& filename)
{
	std::string newFileName = filename.substr(0, filename.size() - 8);
	material->swapIrradianceMap(newFileName.c_str());
}

void OutputNodePBR::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 255, 80, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 255, 0, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 0, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Output node PBR");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(0);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("Albedo color", glm::value_ptr(material->material.albedo));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(1);
	ImGui::SetNextItemWidth(180);
	ImGui::SliderFloat("Metallicness", &(material->material.metallic), 0.0f, 1.0f, "%.2f");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(2);
	ImGui::SetNextItemWidth(180);
	ImGui::SliderFloat("Roughness", &(material->material.roughness), 0.0f, 1.0f, "%.2f");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(3);
	ImGui::Text("Normal");
	ImNodes::EndOutputAttribute();

	if (fileDialog.HasSelected())
	{
		if (textureSelectionMode == BrdfLUT) {
			//ImGui_ImplVulkan_RemoveTexture(material->brdfLUT->DS);
			swapBRDF(fileDialog.GetSelected().string().c_str());
			material->brdfLUT->DS = ImGui_ImplVulkan_AddTexture(material->brdfLUT->getTextureSampler(), material->brdfLUT->getTextureImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}

		if (textureSelectionMode == SpecularMap) {
			swapSpecularMap(fileDialog.GetSelected().string());
		}

		if (textureSelectionMode == DiffuseMap) {
			swapDiffuseMap(fileDialog.GetSelected().string());
		}

		fileDialog.ClearSelected();
	}

	ImGui::TextUnformatted("BRDF LUT");
	ImGui::Image((ImTextureID)material->brdfLUT->DS, ImVec2(150, 150));

	if (ImGui::Button("Select BRDF LUT")) {
		fileDialog.SetTitle("Pick an image");
		fileDialog.SetTypeFilters({ ".png", ".jpg" });
		fileDialog.Open();
		textureSelectionMode = BrdfLUT;
	}
	if (ImGui::Button("Select Specular map")) {
		fileDialog.SetTitle("Pick an image");
		fileDialog.SetTypeFilters({ ".hdr"});
		fileDialog.Open();
		textureSelectionMode = SpecularMap;
	}
	if (ImGui::Button("Select Diffuse map")) {
		fileDialog.SetTitle("Pick an image");
		fileDialog.SetTypeFilters({ ".hdr" });
		fileDialog.Open();
		textureSelectionMode = DiffuseMap;
	}

	fileDialog.Display();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
