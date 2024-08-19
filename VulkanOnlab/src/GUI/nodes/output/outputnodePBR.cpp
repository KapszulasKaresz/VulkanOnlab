#include "GUI/nodes/output/outputnodePBR.h"
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
	std::string ret;

	ret = std::string("#version 460") +
		(rayTracedShadows ? "\n#extension GL_EXT_ray_query : enable\n" : "") +
		"\n" +
		"\nstruct Light {" +
		"\n\tvec4 pos;" +
		"\n\tvec3 La;" +
		"\n\tvec3 Le;" +
		"\n};" +
		"\n" +
		"\n" +
		"\nlayout(set = 0, binding = 0) uniform UniformBufferObject {" +
		"\n\tmat4 view;" +
		"\n\tmat4 proj;" +
		"\n\tvec3 wEye;" +
		"\n\tLight lights[20];" +
		"\n\tint numLights;" +
		"\n} ubo;" +
		(rayTracedShadows ? "\nlayout(set = 0, binding = 1) uniform accelerationStructureEXT topLevelAS;\n" : "") +
		"\n" +
		"\nlayout(set = 2, binding = 0) uniform ObjectUniformBufferObject {" +
		"\n\tmat4 model;" +
		"\n\tmat4 modelInverse;"+
		"\n} oubo;" +
		"\n" +
		"\nlayout(set = 1, binding = 0) uniform Material {" +
		"\n\tvec3 albedo;" +
		"\n\tfloat metallic;" +
		"\n\tfloat roughness;" +
		"\n\tfloat maxMipLevel;" +
		"\n} mat;" +
		"\n" +
		"\nlayout(set = 1, binding = 1) uniform sampler2D brdfLUT;" +
		"\nlayout(set = 1, binding = 2) uniform samplerCube envMap;" +
		"\nlayout(set = 1, binding = 3) uniform samplerCube irradianceMap;" +
		"\n" +
		"\nlayout(location = 0) in vec3 wNormal;" +
		"\nlayout(location = 1) in vec3 wView;" +
		"\nlayout(location = 3) in vec4 wPos;" +
		"\nlayout(location = 4) in vec2 texCoord;" +
		"\nlayout(location = 5) in mat3 TBN;" +
		"\n" +
		"\nlayout(location = 0) out vec4 outColor;" +
		"\n" +
		"\n#define PI 3.1415926\n";


	return ret;
}

std::string OutputNodePBR::getOutputShaderCode(int ouputId)
{
	std::string ret;

	if (inputs[3].first == NULL) {
		ret += "\tvec3 N = normalize(wNormal);\n";
	}
	else {
		ret += "\tvec3 N = ";
		ret += inputs[3].first->getOutputShaderCode(inputs[3].second) + ".xyz;\n";
		ret += "\tN = N * 2.0 - vec3(1.0);\n";
		ret += "\tN = normalize(TBN * N);\n";
	}

	ret += "\tvec3 albedo = ";

	if (inputs[0].first == NULL) {
		ret += "mat.albedo;";
	}
	else {
		ret += inputs[0].first->getOutputShaderCode(inputs[0].second) + ".xyz;";
	}

	ret += "\n";

	ret += "\tfloat metallic = ";

	if (inputs[1].first == NULL) {
		ret += "mat.metallic;";
	}
	else {
		ret += inputs[1].first->getOutputShaderCode(inputs[1].second) + ";";
	}

	ret += "\n";

	ret += "\tfloat roughness = ";

	if (inputs[2].first == NULL) {
		ret += "mat.roughness;";
	}
	else {
		ret += inputs[2].first->getOutputShaderCode(inputs[2].second) + ";";
	}

	ret += "\n\troughness = clamp(roughness, 0.001, 1.0);\n";


	ret += "\n\n";

	ret += std::string("vec3 V = normalize(wView);\n") +
		"\tfloat NdV = max(0.001, dot(N, V));\n" +
		"\tvec3 specular = mix(vec3(0.04), albedo, metallic);\n" +
		"\n" +
		"\tvec3 radiance = vec3(0);\n" +
		"\tvec3 reflectedLight = vec3(0);\n" +
		"\tvec3 diffuseLight = vec3(0);\n" +
		"\n" +
		"\tfor(int i = 0; i < ubo.numLights; i++) {\n" +
		"\t\tvec3 wLight = ubo.lights[i].pos.xyz * wPos.w - wPos.xyz * ubo.lights[i].pos.w;\n" +
		"\t\tvec3 L = normalize(wLight);\n" +
		"\t\tvec3 H = normalize(L + V);\n" +
		"\t\t\n" +
		"\t\tfloat NdL = max(0.0, dot(N, L));\n" +
		"\t\tfloat NdH = max(0.001, dot(N, H));\n" +
		"\t\tfloat HdV = max(0.001, dot(H, V));\n" +
		"\t\tfloat LdV = max(0.001, dot(L, V));\n" +
		"\n" +
		"\t\tvec3 specfresnel = fresnel(specular, HdV);\n" +
		"\t\tvec3 specref = cooktorranceSpecular(NdL, NdV, NdH, specfresnel, roughness);\n" +
		"\n" +
		"\t\tspecref *= vec3(NdL);\n" +
		"\n" +
		"\t\tvec3 diffref = (vec3(1.0) - specfresnel) * phongDiffuse() * NdL;\n" +
		"\n" +
		"\t\tfloat dist = length(wLight);\n" +
		"\t\tif(ubo.lights[i].pos.w < 0.5) {\n" +
		"\t\t\tdist = 1.0f;\n" +
		"\t\t}\n" +
		"\n" +
		(rayTracedShadows ? "\t\tif(!intersects_light(L, wPos.xyz)) {\n" : "") +
		"\t\tvec3 lightColor = ubo.lights[i].Le * 20 / (dist * dist);\n" +
		"\t\treflectedLight += specref * lightColor;\n" +
		"\t\tdiffuseLight += diffref * lightColor;\n" +
		(rayTracedShadows ? "\t\t}\n" : "") +
		"\t}\n" +
		"\n" +
		"\tvec3 envdiff = textureLod(irradianceMap, N, 0.0f).xyz;\n" +
		"\n" +
		"\tvec3 refl = reflect(-V, N);\n" +
		"\tvec3 envspec = textureLod(envMap, refl, max(roughness * mat.maxMipLevel, textureQueryLod(envMap, refl).y)).xyz;\n" +
		"\n" +
		"\tvec2 brdf = texture(brdfLUT, vec2(roughness, 1.0 - NdV)).xy;\n" +
		"\tvec3 iblspec = min(vec3(0.99), fresnel(specular, NdV) * brdf.x + brdf.y);\n" +
		"\treflectedLight += iblspec * envspec;\n" +
		"\tdiffuseLight += envdiff * (1.0 / (PI));\n" +
		"\t\n" +
		"\tradiance = diffuseLight * mix(albedo, vec3(0.0), metallic) + reflectedLight;\n" +
		"\n" +
		"\n" +
		"\toutColor = vec4(radiance, 1.0);\n";


	return ret;
}

std::string OutputNodePBR::getFunctionDefinitions()
{
	std::string ret;

	ret = std::string("\nfloat phongDiffuse()") +
		"\n{" +
		"\n\treturn (1.0 / PI);" +
		"\n}" +
		"\n" +
		"\nvec3 fresnel(vec3 F0, float product) {" +
		"\n\treturn mix(F0, vec3(1.0), pow(1.01 - product, 5.0));" +
		"\n}" +
		"\n" +
		"\nfloat D_GGX(float roughness, float NdH) {" +
		"\n\tfloat m2 = roughness * roughness * roughness * roughness;" +
		"\n\tfloat d = (NdH * m2 - NdH) * NdH + 1.0;" +
		"\n\treturn m2 / (PI * d * d);" +
		"\n}" +
		"\n" +
		"\nfloat G_schlick(float roughness, float NdV, float NdL) {" +
		"\n\tfloat k = roughness * roughness * 0.5;" +
		"\n\tfloat V = NdV * (1.0 - k) + k;" +
		"\n\tfloat L = NdL * (1.0 - k) + k;" +
		"\n\treturn 0.25 / (V * L);" +
		"\n}" +
		"\n" +
		"\nvec3 blinnSpecular(in float NdH, in vec3 specular, in float roughness)" +
		"\n{" +
		"\n\tfloat k = 1.999 / (roughness * roughness);" +
		"\n\t" +
		"\n\treturn min(1.0, 3.0 * 0.0398 * k) * pow(NdH, min(10000.0, k)) * specular;" +
		"\n}" +
		"\n" +
		"\nvec3 cooktorranceSpecular(float NdL, float NdV, float NdH, vec3 specular, float roughness) {" +
		"\n\tfloat D = D_GGX(roughness, NdH);" +
		"\n\tfloat G = G_schlick(roughness, NdV, NdL);" +
		"\n" +
		"\n\treturn specular * G * D;" +
		"\n}\n" + (rayTracedShadows ? 
		"bool intersects_light(vec3 direction, vec3 pos)\n"
		"{\n"
		"\tconst float tmin = 0.01, tmax = 1000;\n"
		"\trayQueryEXT query;\n"
		"\trayQueryInitializeEXT(query, topLevelAS, gl_RayFlagsTerminateOnFirstHitEXT, 0x01, pos, tmin, direction.xyz, tmax);\n"
		"\trayQueryProceedEXT(query);\n"
		"\tif (rayQueryGetIntersectionTypeEXT(query, true) != gl_RayQueryCommittedIntersectionNoneEXT)\n"
		"\t{\n"
		"\t\treturn true;\n"
		"\t}\n"
		"\t\treturn false;\n"
		"\t}\n" : "");

	return ret;
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

	ImGui::Checkbox("Ray-traced shadows", &rayTracedShadows);

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

	ImGui::Text("BRDF LUT");
	
	ImGui::Image((ImTextureID)material->brdfLUT->DS, ImVec2(150, 150));

	if (ImGui::Button("Select BRDF LUT")) {
		fileDialog.SetTitle("Pick an image");
		fileDialog.SetTypeFilters({ ".png", ".jpg" });
		fileDialog.Open();
		textureSelectionMode = BrdfLUT;
	}


	ImGui::Text("Specular map");

	if (ImGui::Button("Select Specular map")) {
		fileDialog.SetTitle("Pick an image");
		fileDialog.SetTypeFilters({ ".hdr" });
		fileDialog.Open();
		textureSelectionMode = SpecularMap;
	}

	if (ImGui::Button("Preview Specular")) {
		cubeMapPreviewer.open(material->cubeMap);
	}
		
	ImGui::Text("Diffuse map");

	if (ImGui::Button("Select Diffuse map")) {
		fileDialog.SetTitle("Pick an image");
		fileDialog.SetTypeFilters({ ".hdr" });
		fileDialog.Open();
		textureSelectionMode = DiffuseMap;
	}

	if (ImGui::Button("Preview Irradiance")) {
		cubeMapPreviewer.open(material->irradianceMap);
	}
	

	fileDialog.Display();
	cubeMapPreviewer.draw();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
