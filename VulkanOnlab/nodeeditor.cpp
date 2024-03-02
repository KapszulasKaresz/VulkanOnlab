#include "nodeeditor.h"
#include "outputnodePhong.h"
#include "outputnodePBR.h"
#include <iostream>
#include <fstream>
#include "object.h"
#include <shaderc/shaderc.hpp> 
#include <iterator>

NodeEditor::NodeEditor(Material* material, Object* object) : material(material), sharedGraphInfo(material->graphInfo), object(object) {
	outputNode = new OutputNodePhong(material); 
	fragShaderName = std::string("shaders/outputPhongFrag") + std::to_string(object->id);
}

void NodeEditor::open(std::string& name)
{
	display = true;
	this->name = name;
}

void NodeEditor::close()
{
	display = false;
}

void NodeEditor::draw()
{
	if (display) {
		if (!ImGui::Begin(name.c_str(), &display, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse)) {
			ImGui::End();
		}

		if (ImGui::BeginMenuBar()) {

			if (ImGui::BeginMenu("Add")) {
				if (ImGui::BeginMenu("Input")) {
					const char* namesInput[] = {
					"Texture node",
					"Const color picker"
					};

					for (int i = 0; i < 2; i++)
					{
						if (ImGui::MenuItem(namesInput[i]))
							if (namesInput[i] == "Texture node") {
								TextureNode* node = new TextureNode(nodeId++, sharedGraphInfo);
								nodes.push_back(node);
								textureNodes.push_back(node);
							}
							else if (namesInput[i] == "Const color picker") {
								ColorConstNode* node = new ColorConstNode(nodeId++);
								nodes.push_back(node);
								colorConstNodes.push_back(node);
							}

					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Modify")) {
					const char* names[] = {
						"Mixer node",
						"Masking node"
					};

					for (int i = 0; i < 2; i++)
					{
						if (ImGui::MenuItem(names[i]))
							if (names[i] == "Mixer node") {
								MixerNode* node = new MixerNode(nodeId++);
								nodes.push_back(node);
								mixerNodes.push_back(node);
							} else if (names[i] == "Masking node") {
								MaskingNode* node = new MaskingNode(nodeId++);
								nodes.push_back(node);
								maskingNodes.push_back(node);
							}

					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Output")) {
					const char* namesOutput[] = {
					"Phong-Bling",
					"PBR"
					};

					for (int i = 0; i < 2; i++)
					{
						if (ImGui::MenuItem(namesOutput[i]))
							if (namesOutput[i] == "Phong-Bling") {
								delete outputNode;
								outputNode = new OutputNodePhong(material);
								renderingMode = Phong;
								fragShaderName = std::string("shaders/outputPhongFrag") + std::to_string(object->id);
							}
							else if (namesOutput[i] == "PBR") {
								delete outputNode;
								outputNode = new OutputNodePBR(material); 
								renderingMode = PBR;
								fragShaderName = std::string("shaders/outputPBRFrag") + std::to_string(object->id);
							}
					}

					ImGui::EndMenu();
				}


				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Apply")) {
				generateShaderCode();
				std::vector<Texture*> textures;

				for (int i = 0; i < textureNodes.size(); i++) {
					textures.push_back(textureNodes[i]->getTexture());
					textureNodes[i]->hasBeenAssigned = true;
				}

				material->setTexture(textures);
				material->recreateDescriptors();
				std::string filename = fragShaderName + ".spv";
				object->recreatePipeline(filename.c_str());
			}
			ImGui::EndMenuBar();
		}

		ImNodes::BeginNodeEditor();
		
		for (Node* node : nodes) {
			node->draw();
		}

		outputNode->draw();

		for (int i = 0; i < links.size(); i++) {
			const std::pair<int, int> p = links[i];

			ImNodes::Link(i, p.first, p.second);
		}

		ImNodes::EndNodeEditor(); 

		int id;
		if (ImNodes::IsLinkDestroyed(&id)) {
			links.erase(links.begin() + id);
		}

		int start_attr, end_attr;  
		if (ImNodes::IsLinkCreated(&start_attr, &end_attr)) 
		{
			links.push_back(std::make_pair(start_attr, end_attr)); 
		}

		ImGui::End();
	}
}

void NodeEditor::generateShaderCode()
{
	if (renderingMode == Phong) {
		std::string filename = fragShaderName + ".frag";
		std::ofstream outFile(filename.c_str());

		outFile << "#version 450\n\n"
				<< "struct Light {\n"
				<< "\tvec4 pos;\n"
				<< "\tvec3 La;\n"
				<< "\tvec3 Le;\n"
				<< "};\n\n"
				<< "layout(set = 0, binding = 0) uniform UniformBufferObject {\n"
				<< "\tmat4 view;\n"
				<< "\tmat4 proj;\n"
				<< "\tvec3 wEye;\n"
				<< "\tLight lights[20];\n"
				<< "\tint numLights;\n"
				<< "} ubo;\n"
				<< "\n"
				<< "layout(set = 2, binding = 0) uniform ObjectUniformBufferObject {\n"
				<< "\tmat4 model;\n"
				<< "\tmat4 modelInverse;\n"
				<< "} oubo;\n"
				<< "\n"
				<< "\n"
				<< "layout(location = 0) in vec3 wNormal;\n"
				<< "layout(location = 1) in vec3 wView;\n"
				<< "layout(location = 3) in vec4 wPos;\n"
				<< "layout(location = 4) in vec2 texCoord;\n"
				<< "\n"
				<< "layout(location = 0) out vec4 outColor;\n\n"
				<< "layout(set = 1, binding = 0) uniform Material {\n"
				<< "\tfloat shininess;\n"
				<< "\tvec3 ks;\n"
				<< "\tvec3 kd;\n"
				<< "\tvec3 ka;\n"
				<< "} mat;\n\n";


		for (int i = 0; i < textureNodes.size(); i++) {
			outFile << "layout(set = 1, binding = " << i + 1 << ") uniform sampler2D texSampler" << textureNodes[i]->getId()<<";\n";
		}

		outFile << "\n"
				<< "void main() {\n"
				<< "\tvec3 N = normalize(wNormal);\n"
				<< "\tvec3 V = normalize(wView); \n";
				
		outFile<< "\tvec3 kd = ";

		std::string returned = getColorInput(0);
		std::string kd =returned == " " ? "mat.kd" : returned;


		outFile << kd << ";\n";

		outFile << "\tvec3 ka = ";

		returned = getColorInput(1);
		std::string ka = returned == " " ? "mat.ka" : returned;

		outFile << ka << ";\n\n";

		//TODO MIXER AND MATH
		//TODO NORMAL MAP
				
		outFile << "\tvec3 radiance = vec3(0, 0, 0);\n"
				<< "\n"
				<< "\tfor(int i = 0; i < ubo.numLights; i++){\n"
				<< "\t\tvec3 wLight = ubo.lights[i].pos.xyz * wPos.w - wPos.xyz * ubo.lights[i].pos.w;\n"
				<< "\t\tvec3 L = normalize(wLight);\n"
				<< "\t\tvec3 H = normalize(L + V);\n"
				<< "\t\tfloat cost = max(dot(N,L), 0), cosd = max(dot(N,H), 0);\n"
				<< "\t\tfloat dist = length(wLight);\n"
				<< "\t\tif(ubo.lights[i].pos.w < 0.5) {\n"
				<< "\t\t\tdist = 1.0f;\n"
				<< "\t\t}\n"
				<< "\n"
				<< "\t\tradiance += (ka * ubo.lights[i].La + (kd * cost + mat.ks * pow(cosd, mat.shininess)) * ubo.lights[i].Le) / (dist* dist);\n"
				<< "\t}\n"
				<< "\toutColor = vec4(radiance, 1.0);\n"
				<< "}\n";



		outFile.close();

		std::ifstream t(filename.c_str());
		std::stringstream buffer;
		buffer << t.rdbuf();

		std::string filenameout = fragShaderName + ".spv";

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		
		shaderc::SpvCompilationResult  result = compiler.CompileGlslToSpv(buffer.str().c_str(), shaderc_glsl_fragment_shader, filename.c_str(), options);

		std::vector<uint32_t> assembly(result.begin(), result.end());

		std::ofstream outSPIRV(filenameout.c_str(), std::ios::binary);
		
		outSPIRV.write(reinterpret_cast<const char*>(assembly.data()), assembly.size() * sizeof(uint32_t));

		outSPIRV.close();	
	}
}

NodeEditor::~NodeEditor()
{
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
	delete outputNode;
}

std::string NodeEditor::getColorInput(int id)
{
	std::string ret = " ";

	for (int i = 0; i < links.size(); i++) {
		const std::pair<int, int> p = links[i];
		if (p.second == id) {
			for (int i = 0; i < textureNodes.size(); i++) {
				if (p.first == textureNodes[i]->getId() * 10) {
					ret = std::string("texture(texSampler") + std::to_string(textureNodes[i]->getId()) + std::string(", texCoord).xyz");
					return ret;
				}
			}

			for (int i = 0; i < colorConstNodes.size(); i++) {
				if (p.first == colorConstNodes[i]->getId() * 10) {
					ret = std::string("vec3(") + std::to_string(colorConstNodes[i]->color.x) + std::string(", ") + std::to_string(colorConstNodes[i]->color.y) + std::string(", ") + std::to_string(colorConstNodes[i]->color.z) + std::string(")");
					return ret;
				}
			}

			for (int i = 0; i < mixerNodes.size(); i++) {
				if ((p.first - (p.first % 10)) == mixerNodes[i]->getId() * 10) {
					ret = std::string("mix(") + getColorInput(mixerNodes[i]->getId() *10 + 0) + std::string(", ") + getColorInput(mixerNodes[i]->getId() *10 + 2) + std::string(", ")
						+ std::string("vec3(") + std::to_string(mixerNodes[i]->getMix()) + std::string(", ") + std::to_string(mixerNodes[i]->getMix()) + std::string(", ") + std::to_string(mixerNodes[i]->getMix()) + std::string("))");
					return ret;
				}
			}

			for (int i = 0; i < maskingNodes.size(); i++) {
				if ((p.first - (p.first % 10)) == maskingNodes[i]->getId() * 10) {
					ret = getColorInput(maskingNodes[i]->getId() * 10 + 0) + std::string(" * ") + getColorInput(maskingNodes[i]->getId() * 10 + 1);
					return ret;
				}
			}

		}

	}

	return ret;
}
