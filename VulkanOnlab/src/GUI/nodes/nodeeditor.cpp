#include "GUI/nodes/nodeeditor.h"
#include "GUI/nodes/output/outputnodePhong.h"
#include "GUI/nodes/output/outputnodePBR.h"
#include "GUI/nodes/output/outputnodeEnvMap.h"
#include "GUI/nodes/input/floatpickernode.h"
#include "GUI/nodes/edit/invertednode.h"
#include "GUI/nodes/edit/invertednode.h"
#include "GUI/nodes/edit/customcodenode.h"
#include "../src/GUI/nodes/edit/vecassemblernode.cpp"
#include "../src/GUI/nodes/edit/vecdisassemblernode.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "vulkan/object/object.h"
#include "vulkan/material/materialstore.h"
#include <shaderc/shaderc.hpp> 
#include <iterator>

NodeEditor::NodeEditor(Material* material) : material(material) {
	outputNode = new OutputNodePhong(dynamic_cast<MaterialPhong*>(material));
	nodes.push_back(outputNode);
	fragShaderName = std::string("res/shaders/outputPhongFrag") + std::to_string(material->id);
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
				if (renderingMode != EnvMap && ImGui::BeginMenu("Input") ) {
					const char* namesInput[] = {
					"Texture node",
					"Checkered Texture node",
					"Const color picker",
					"Float constant",
					"Position node"
					};

					for (int i = 0; i < 5; i++)
					{
						if (ImGui::MenuItem(namesInput[i]))
							if (namesInput[i] == "Texture node") {
								TextureNode* node = new TextureNode(nodeId++);
								nodes.push_back(node);
								textureNodes.push_back(node);
							}
							else if (namesInput[i] == "Checkered Texture node") {
								CheckeredTextureNode* node = new CheckeredTextureNode(nodeId++);
								nodes.push_back(node);
								textureNodes.push_back(node);
							}
							else if (namesInput[i] == "Const color picker") {
								ColorConstNode* node = new ColorConstNode(nodeId++);
								nodes.push_back(node);
							}
							else if (namesInput[i] == "Float constant") {
								FloatNode* node = new FloatNode(nodeId++);
								nodes.push_back(node);
							}
							else if (namesInput[i] == "Position node") {
								PositionNode* node = new PositionNode(nodeId++);
								nodes.push_back(node);
							}

					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Vector")) {
					if (ImGui::BeginMenu("Assemble")) {
						const char* names[] = {
							"vec4",
							"vec3",
							"vec2"
						};

						for (int i = 0; i < 3; i++) {
							if (ImGui::MenuItem(names[i])) {
								if (names[i] == "vec4") {
									VecAssemblerNode<4>* node = new VecAssemblerNode<4>(nodeId++);
									nodes.push_back(node);
								}
								else if (names[i] == "vec3") {
									VecAssemblerNode<3>* node = new VecAssemblerNode<3>(nodeId++);
									nodes.push_back(node);
								}
								else if (names[i] == "vec2") {
									VecAssemblerNode<2>* node = new VecAssemblerNode<2>(nodeId++);
									nodes.push_back(node);
								}
							}
						}

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Dissassemble")) {
						const char* names[] = {
							"vec4",
							"vec3",
							"vec2"
						};

						for (int i = 0; i < 3; i++) {
							if (ImGui::MenuItem(names[i])) {
								if (names[i] == "vec4") {
									VecDisassemblerNode<4>* node = new VecDisassemblerNode<4>(nodeId++);
									nodes.push_back(node);
								}
								else if (names[i] == "vec3") {
									VecDisassemblerNode<3>* node = new VecDisassemblerNode<3>(nodeId++);
									nodes.push_back(node);
								}
								else if (names[i] == "vec2") {
									VecDisassemblerNode<2>* node = new VecDisassemblerNode<2>(nodeId++);
									nodes.push_back(node);
								}
							}
						}

						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Modify")) {
					const char* names[] = {
						"Mixer node",
						"Masking node",
						"Inverter node",
						"Math node",
						"Custom Code node"
					};

					for (int i = 0; i < 5; i++)
					{
						if (ImGui::MenuItem(names[i])) {
							if (names[i] == "Mixer node") {
								MixerNode* node = new MixerNode(nodeId++);
								nodes.push_back(node);
							}
							else if (names[i] == "Masking node") {
								MaskingNode* node = new MaskingNode(nodeId++);
								nodes.push_back(node);
							}
							else if (names[i] == "Masking node") {
								MaskingNode* node = new MaskingNode(nodeId++);
								nodes.push_back(node);
							}
							else if (names[i] == "Inverter node") {
								InverterNode* node = new InverterNode(nodeId++);
								nodes.push_back(node);
							}
							else if (names[i] == "Math node") {
								MathNode* node = new MathNode(nodeId++);
								nodes.push_back(node);
							}
							else if (names[i] == "Custom Code node") {
								CustomCodeNode* node = new CustomCodeNode(nodeId++);
								nodes.push_back(node);
							}
						}
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Output")) {
					const char* namesOutput[] = {
					"Phong-Bling",
					"PBR",
					"EnvMap"
					};

					for (int i = 0; i < 3; i++)
					{
						if (ImGui::MenuItem(namesOutput[i]))
							if (namesOutput[i] == "Phong-Bling") {
								resetEditor();
								MaterialPhong* newMaterial = new MaterialPhong();
								newMaterial->id = material->id;
								newMaterial->name = material->name;
								MaterialStore::swapMaterial(newMaterial);
								material = newMaterial;
								delete outputNode;
								outputNode = new OutputNodePhong(newMaterial);
								nodes[0] = outputNode;
								renderingMode = Phong;
								fragShaderName = std::string("res/shaders/outputPhongFrag") + std::to_string(material->id);
							}
							else if (namesOutput[i] == "PBR") {
								resetEditor();
								MaterialPBR* newMaterial = new MaterialPBR();
								newMaterial->id = material->id;
								newMaterial->name = material->name;
								MaterialStore::swapMaterial(newMaterial);
								material = newMaterial;
								delete outputNode;
								outputNode = new OutputNodePBR(newMaterial);
								nodes[0] = outputNode;
								renderingMode = PBR;
								fragShaderName = std::string("res/shaders/outputPBRFrag") + std::to_string(material->id);
							}
							else if (namesOutput[i] == "EnvMap") {
								resetEditor();
								MaterialEnvMap* newMaterial = new MaterialEnvMap();
								newMaterial->id = material->id;
								newMaterial->name = material->name;
								MaterialStore::swapMaterial(newMaterial);
								material = newMaterial;
								delete outputNode;
								outputNode = new OutputNodeEnvMap(newMaterial);
								nodes[0] = outputNode;
								renderingMode = EnvMap;
								fragShaderName = std::string("res/shaders/envmap/shader") + std::to_string(material->id);
							}
					}

					ImGui::EndMenu();
				}


				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Apply")) {
				if (renderingMode != EnvMap) {
					generateShaderCode();
				}
				std::vector<Texture*> textures;

				for (int i = 0; i < textureNodes.size(); i++) {
					textures.push_back(textureNodes[i]->getTexture());
					textureNodes[i]->hasBeenAssigned = true;
				}

				material->setTexture(textures);
				material->recreateDescriptors();
				std::string filename = fragShaderName + ".spv";
				material->recreatePipeline(filename.c_str());
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete))) {
			for (int i = 0; i < textureNodes.size(); i++) {
				if (ImNodes::IsNodeSelected(textureNodes[i]->getId())) {
					textureNodes.erase(textureNodes.begin() + i);
					i--;
				}
			}
			for (int i = 0; i < nodes.size(); i++) {
				if (ImNodes::IsNodeSelected(nodes[i]->getId())) {
					deleteNode(nodes[i]->getId());
					delete nodes[i];
					nodes.erase(nodes.begin() + i);
					i--;
				}
			}
		}

		ImNodes::BeginNodeEditor();

		for (Node* node : nodes) {
			node->draw();
		}

		for (int i = 0; i < links.size(); i++) {
			const std::pair<int, int> p = links[i];

			ImNodes::Link(i, p.first, p.second);
		}

		ImNodes::EndNodeEditor();

		int id;
		if (ImNodes::IsLinkDestroyed(&id)) {
			deleteLinkFromNode(links[id].first, links[id].second);
			links.erase(links.begin() + id);
		}

		int start_attr, end_attr;
		if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
		{
			links.push_back(std::make_pair(start_attr, end_attr));
			newLinkToNodes(start_attr, end_attr);
		}

		ImGui::End();
	}
}

void NodeEditor::generateShaderCode()
{
	std::string filename = fragShaderName + ".frag";
	std::ofstream outFile(filename.c_str());


	outFile << outputNode->getShaderCodeUniforms();

	for (int i = 0; i < textureNodes.size(); i++) {
		outFile << "layout(set = 1, binding = " << i + outputNode->getPreBindedResourceCount() << ") uniform sampler2D texSampler" << textureNodes[i]->getId() << ";\n";
	}

	for (int i = 0; i < nodes.size(); i++) {
		outFile << nodes[i]->getFunctionDefinitions();
	}

	outFile << "\n"
		<< "void main() {\n"
		<< outputNode->getOutputShaderCode(0)
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

NodeEditor::~NodeEditor()
{
	for (int i = 0; i < nodes.size(); i++) {
		delete nodes[i];
	}
}

void NodeEditor::deleteNode(int id)
{
	for (int i = 0; i < links.size(); i++) {
		const std::pair<int, int> p = links[i];

		if (p.first - (p.first % 10) == id * 10 || p.second - (p.second % 10) == id * 10) {
			deleteLinkFromNode(p.first, p.second);
			links.erase(links.begin() + i);
			i--;
		}
	}
}

void NodeEditor::newLinkToNodes(int startAttrib, int endAttrib)
{
	for (Node* node_end : nodes) {
		if (node_end->getId() * 10 == endAttrib - (endAttrib % 10)) {
			for (Node* node_start : nodes) {
				if (node_start->getId() * 10 == startAttrib - (startAttrib % 10)) {
					node_end->inputs[endAttrib % 10] = std::make_pair(node_start, startAttrib % 10);
				}
			}
		}
	}
}

void NodeEditor::deleteLinkFromNode(int startAttrib, int endAttrib)
{
	for (Node* node_end : nodes) {
		if (node_end->getId() * 10 == endAttrib - (endAttrib % 10)) {
			for (Node* node_start : nodes) {
				if (node_start->getId() * 10 == startAttrib - (startAttrib % 10)) {
					node_end->inputs.erase(endAttrib % 10);
				}
			}
		}
	}
}

void NodeEditor::resetEditor()
{
	for (int i = 1; i < nodes.size(); i++) {
		deleteNode(nodes[i]->getId());
		delete nodes[i];
	}
	textureNodes.clear();
	nodes.clear();
	nodes.push_back(outputNode);
}
