#include "GUI/nodes/outputnodePhong.h"
#include <glm/gtc/type_ptr.hpp>

OutputNodePhong::OutputNodePhong(Material* material) : OutputNode(material)
{
	ImNodes::SetNodeGridSpacePos(getId(), ImVec2(400, 0));
}

std::string OutputNodePhong::getOutputShaderCode(int ouputId)
{
	std::string ret;

	if (inputs[4].first == NULL) {
		ret += "\tvec3 N = normalize(wNormal);\n";
	}
	else {
		ret += "\tvec3 N = ";
		ret += inputs[4].first->getOutputShaderCode(inputs[4].second) + ".xyz;\n";
		ret += "\tN = N * 2.0 - vec3(1.0);\n";
		ret += "\tN = normalize(TBN * N);\n";
	}

	ret += "\tvec3 V = normalize(wView); \n";
	ret += "\tvec3 kd = ";

	if (inputs[0].first == NULL) {
		ret += "mat.kd";
	}
	else {
		ret += inputs[0].first->getOutputShaderCode(inputs[0].second) + ".xyz";
	}

	ret += ";\n";

	ret += "\tvec3 ka = ";

	if (inputs[1].first == NULL) {
		ret += "mat.ka";
	}
	else {
		ret += inputs[1].first->getOutputShaderCode(inputs[1].second) + ".xyz";
	}

	ret += ";\n";

	ret += "\tvec3 ks = ";

	if (inputs[2].first == NULL) {
		ret += "mat.ks";
	}
	else {
		ret += inputs[2].first->getOutputShaderCode(inputs[2].second) + ".xyz";
	}

	ret += ";\n";

	ret += "\tfloat shininess = ";

	if (inputs[3].first == NULL) {
		ret += "mat.shininess";
	}
	else {
		ret += inputs[3].first->getOutputShaderCode(inputs[3].second);
	}

	ret += ";\n\n";

	ret += std::string("\tvec3 radiance = vec3(0, 0, 0);\n")
		+ "\n"
		+ "\tfor(int i = 0; i < ubo.numLights; i++){\n"
		+ "\t\tvec3 wLight = ubo.lights[i].pos.xyz * wPos.w - wPos.xyz * ubo.lights[i].pos.w;\n"
		+ "\t\tvec3 L = normalize(wLight);\n"
		+ "\t\tvec3 H = normalize(L + V);\n"
		+ "\t\tfloat cost = max(dot(N,L), 0), cosd = max(dot(N,H), 0);\n"
		+ "\t\tfloat dist = length(wLight);\n"
		+ "\t\tif(ubo.lights[i].pos.w < 0.5) {\n"
		+ "\t\t\tdist = 1.0f;\n"
		+ "\t\t}\n"
		+ "\n"
		+ "\t\tradiance += (ka * ubo.lights[i].La + (kd * cost + ks * pow(cosd, shininess)) * ubo.lights[i].Le) / (dist* dist);\n"
		+ "\t}\n"
		+ "\toutColor = vec4(radiance, 1.0);\n";

	return ret;
}

std::string OutputNodePhong::getShaderCodeUniforms()
{
	std::string ret;

	ret += std::string("#version 450\n\n")
		+ "struct Light {\n"
		+ "\tvec4 pos;\n"
		+ "\tvec3 La;\n"
		+ "\tvec3 Le;\n"
		+ "};\n\n"
		+ "layout(set = 0, binding = 0) uniform UniformBufferObject {\n"
		+ "\tmat4 view;\n"
		+ "\tmat4 proj;\n"
		+ "\tvec3 wEye;\n"
		+ "\tLight lights[20];\n"
		+ "\tint numLights;\n"
		+ "} ubo;\n"
		+ "\n"
		+ "layout(set = 2, binding = 0) uniform ObjectUniformBufferObject {\n"
		+ "\tmat4 model;\n"
		+ "\tmat4 modelInverse;\n"
		+ "} oubo;\n"
		+ "\n"
		+ "\n"
		+ "layout(location = 0) in vec3 wNormal;\n"
		+ "layout(location = 1) in vec3 wView;\n"
		+ "layout(location = 3) in vec4 wPos;\n"
		+ "layout(location = 4) in vec2 texCoord;\n"
		+ "layout(location = 5) in mat3 TBN;\n"
		+ "\n"
		+ "layout(location = 0) out vec4 outColor;\n\n"
		+ "layout(set = 1, binding = 0) uniform Material {\n"
		+ "\tfloat shininess;\n"
		+ "\tvec3 ks;\n"
		+ "\tvec3 kd;\n"
		+ "\tvec3 ka;\n"
		+ "} mat;\n\n";

	return ret;
}

void OutputNodePhong::draw()
{
	ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(0, 255, 80, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarHovered, IM_COL32(0, 255, 0, 255));
	ImNodes::PushColorStyle(ImNodesCol_TitleBarSelected, IM_COL32(0, 255, 0, 255));
	ImNodes::BeginNode(getId());
	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted("Output node");
	ImNodes::EndNodeTitleBar();

	ImNodes::BeginInputAttribute(0);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("kd", glm::value_ptr(getMaterial()->material.kd));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(1);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("ka", glm::value_ptr(getMaterial()->material.ka));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(2);
	ImGui::SetNextItemWidth(180);
	ImGui::ColorEdit3("ks", glm::value_ptr(getMaterial()->material.ks));
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(3);
	ImGui::SetNextItemWidth(180);
	ImGui::SliderFloat("shininess", &(getMaterial()->material.shininess), 0.0f, 100.0f, "%.2f");
	ImNodes::EndOutputAttribute();

	ImNodes::BeginInputAttribute(4);
	ImGui::Text("Normal");
	ImNodes::EndOutputAttribute();

	ImNodes::EndNode();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
	ImNodes::PopColorStyle();
}
