#include "vulkan/scene.h"
#include "vulkan/application.h"
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_NOEXCEPTION
#define JSON_NOEXCEPTION
#include <tinygltf/tiny_gltf.h>
#include "GUI/mainmenu.h"
#include "vulkan/material/materialPBR.h"
#include "vulkan/material/materialstore.h"


void Scene::buildScene()
{
	camera.asp = (float)Application::WIDTH / Application::HEIGHT;
	camera.fov = glm::radians(45.0f);
	camera.fp = 200.0f;
	camera.np = 0.1f;
	camera.wForward = glm::normalize(glm::vec3(0.0f) - glm::vec3(5.0f, 5.0f, 5.0f));
	camera.wVup = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.wEye = glm::vec3(5.0f, 5.0f, 5.0f);
	mainMenu = new MainMenu(this);
	createTopLevelAccelerationStructure();
}

void Scene::cleanup()
{
	if (topLevelAS != nullptr) {
		delete topLevelAS;
	}
	delete mainMenu;

	for (int i = 0; i < rootNodes.size(); i++) {
		delete rootNodes[i];
	}

	for (int i = 0; i < lights.size(); i++) {
		delete lights[i];
	}

	vkDestroyBuffer(Application::device, asInstanceBuffer, nullptr);
	vkFreeMemory(Application::device, asInstanceBufferMemory, nullptr);

	vkDestroyDescriptorSetLayout(Application::device, Object::descriptorSetLayout, nullptr);
}

void Scene::loadDummData()
{
	//Object* obj = new Object();
	//obj->create("res/models/icosphere.obj");

	//objects.push_back(obj);

	//topLevelASChanged = true;
	//createASInstanceBuffer();
	//topLevelAS->addInstanceGeometry(asInstanceBuffer, 1);
	//topLevelAS->build();

	//ImGuiObject* imObj = new ImGuiObject(obj, "res/models/cube.obj", this, mainMenu);
	//mainMenu->addObject(imObj);
}

bool Scene::loadGLTFScene(std::filesystem::path path, MainMenu* mainMenu)
{
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	tinygltf::Model model;

	bool res = loader.LoadASCIIFromFile(&model, &err, &warn, path.string().c_str());
	if (!warn.empty()) {
		std::cout << "WARN: " << warn << std::endl;
	}

	if (!err.empty()) {
		std::cout << "ERR: " << err << std::endl;
	}

	if (!res)
		std::cout << "Failed to load glTF: " << path.string() << std::endl;
	else
		std::cout << "Loaded glTF: " << path.string() << std::endl;

	loadGLTFMaterials(path.parent_path(), &model);

	std::vector<RenderNode*> l_nodes;
	std::vector<RenderNode*> l_rootNodes;

	for (int i = 0; i < model.nodes.size(); i++) {
		auto node = model.nodes[i];
		auto currentNode = new RenderNode();
		currentNode->name = "Node: " + model.nodes[i].name;
		currentNode->gltfID = i;
		l_nodes.push_back(currentNode);
		setTransforms(currentNode, &node);
		if (node.mesh != -1)
		{
			for (int j = 0; j < model.meshes[node.mesh].primitives.size(); j++) {
				Object* obj = new Object();
				obj->create(&(model.meshes[node.mesh].primitives[j]), &model);

				currentNode->addChild(obj);

				if (model.meshes[node.mesh].primitives[j].material != -1) {
					for (int k = 0; k < MaterialStore::materials.size(); k++) {
						if (MaterialStore::materials[k].first->gltfId == model.meshes[node.mesh].primitives[j].material) {
							obj->swapMaterial(MaterialStore::materials[k].first);
						}
					}
				}
				obj->name = "Mesh: " +  model.meshes[node.mesh].name;
			}
		}		
	}

	for (int i = 0; i < model.nodes.size(); i++) {
		for (int j = 0; j < model.nodes[i].children.size(); j++) {
			l_nodes[i]->addChild(l_nodes[model.nodes[i].children[j]]);
		}
	}

	for (int i = 0; i < l_nodes.size(); i++) {
		bool insert = true;
		for (int j = 0; j < l_rootNodes.size(); j++) {
			if (l_rootNodes[j]->id == l_nodes[i]->getRootNode()->id) {
				insert = false;
			}
		}
		if (insert) {
			l_rootNodes.push_back(l_nodes[i]->getRootNode());
		}
	}

	rootNodes.insert(rootNodes.end(), l_rootNodes.begin(), l_rootNodes.end());

	return res;
}

void Scene::addRootNode(RenderNode* node)
{
	rootNodes.push_back(node);
}

void Scene::updateUniformBuffer(uint32_t currentImage)
{
	for (Object* object : getObjects()) {

		object->updateUniformBuffer(currentImage, camera, lights);
	}
}

void Scene::addLight()
{
	if (lights.size() < MAX_LIGHTS) {
		lights.push_back(new Light(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(0.2f), glm::vec3(1)));
		ImGuiLight* imLight = new ImGuiLight(lights[lights.size() - 1], this, mainMenu);
		mainMenu->addLight(imLight);
	}
}

void Scene::deleteLight(Light* light)
{
	for (int i = 0; i < lights.size(); i++) {
		if (*(lights[i]) == *light) {
			delete lights[i];
			lights.erase(lights.begin() + i);
			break;
		}
	}
}

void Scene::drawMenu()
{
	mainMenu->draw();

}

void Scene::updateAS()
{
	int updateCount = 0;
	int objectsWithAS = 0;
	auto objects = getObjects();
	for (auto object : objects) {
		object->checkTransformationUpdate();
		if (object->hasAccelerationStructure) {
			objectsWithAS += 1;
			if (object->accelerationStructureDirty) {
				updateCount += 1;
				object->accelerationStructureDirty = false;
			}
		}
	}

	if (updateCount > 0 || deleteObjectWithAS) {
		topLevelASChanged = true;
		deleteObjectWithAS = false;
		if (objectsWithAS > 0) {
			createASInstanceBuffer();

			if (topLevelAS->isEmpty()) {
				topLevelAS->addInstanceGeometry(asInstanceBuffer, objects.size());
			}
			else {
				topLevelAS->updateInstanceGeometry(0, asInstanceBuffer, objects.size());
			}
			topLevelAS->build();
		}
	}
}

void Scene::createTopLevelAccelerationStructure()
{
	topLevelAS = new AccelerationStructure(Application::device, VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR);
}

std::vector<RenderNode*>& Scene::getRootNodes()
{
	return rootNodes;
}

std::vector<Object*> Scene::getObjects()
{
	std::vector<Object*> ret;

	for (int i = 0; i < rootNodes.size(); i++) {
		auto rootObjects = rootNodes[i]->getObjects();
		ret.insert(ret.end(), rootObjects.begin(), rootObjects.end());
	}

	return ret;
}

Scene::~Scene()
{

}

void Scene::loadGLTFMaterials(std::filesystem::path path, tinygltf::Model* gltfModel)
{
	for (int i = 0; i < gltfModel->materials.size(); i++) {
		auto gltfMaterial = gltfModel->materials[i];
		auto material = new MaterialPBR();

		material->doubleSided = gltfMaterial.doubleSided;

		if (gltfMaterial.name != "") {
			material->name = gltfMaterial.name;
		}
		else {
			material->name = "#" + std::to_string(i);
		}

		material->gltfId = i;

		const auto& pbr = gltfMaterial.pbrMetallicRoughness;

		if (!pbr.baseColorFactor.empty()) {
			material->material.albedo = glm::vec3(pbr.baseColorFactor[0], pbr.baseColorFactor[1], pbr.baseColorFactor[2]);
		}

		if (pbr.metallicFactor != -1.0f) {
			material->material.metallic = static_cast<float>(pbr.metallicFactor);
		}

		if (pbr.roughnessFactor != -1.0f) {
			material->material.roughness = static_cast<float>(pbr.roughnessFactor);
		}

		MaterialStore::addMaterial(material);

		NodeEditor* nodeEditor = nullptr;
		for (int j = 0; j < MaterialStore::materials.size(); j++) {
			if (MaterialStore::materials[j].first->id == material->id) {
				nodeEditor = MaterialStore::materials[j].second;
			}
		}

		bool forceApply = false;

		if (pbr.baseColorTexture.index != -1) {
			if (nodeEditor != nullptr) {
				nodeEditor->addAlbedoTexture((path / gltfModel->images[gltfModel->textures[pbr.baseColorTexture.index].source].uri));
				forceApply = true;
			}
		}

		if (pbr.metallicRoughnessTexture.index != -1) {
			if (nodeEditor != nullptr) {
				nodeEditor->addMetallicRoughnessTexture((path / gltfModel->images[gltfModel->textures[pbr.metallicRoughnessTexture.index].source].uri));
				forceApply = true;
			}
		}

		if (gltfMaterial.normalTexture.index != -1) {
			if (nodeEditor != nullptr) {
				nodeEditor->addNormalTexture((path / gltfModel->images[gltfModel->textures[gltfMaterial.normalTexture.index].source].uri));
				forceApply = true;
			}
		}

		if (forceApply) {
			nodeEditor->forceApply();
		}
	}
}

void Scene::setTransforms(RenderNode* renderNode, tinygltf::Node* node)
{
	if (node->matrix.size() != 0) {
		glm::mat4 transformationMatrix = glm::make_mat4(node->matrix.data());
		renderNode->addTransform(new MatrixTransform(transformationMatrix));
	}
	else {

		if (node->scale.size() != 0) {
			renderNode->addTransform(new Scale(
				glm::vec3(node->scale[0],
					node->scale[1],
					node->scale[2])));
		}

		if (node->rotation.size() != 0) {
			renderNode->addTransform(new Rotation(
				glm::vec3(node->rotation[0],
					node->rotation[1],
					node->rotation[2]),
				node->rotation[3]));
		}

		if (node->translation.size() != 0) {
			renderNode->addTransform(new Translation(
				glm::vec3(node->translation[0],
					node->translation[1],
					node->translation[2])));
		}

	}
}

void Scene::createASInstanceBuffer()
{
	auto objects = getObjects();
	std::vector<VkAccelerationStructureInstanceKHR> instances;
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->hasAccelerationStructure) {
			instances.push_back(objects[i]->instance);
		}
	}

	VkDeviceSize instancesSize = instances.size() * sizeof(VkAccelerationStructureInstanceKHR);

	if (asInstanceBuffer != VK_NULL_HANDLE) {
		vkDestroyBuffer(Application::device, asInstanceBuffer, nullptr);
		vkFreeMemory(Application::device, asInstanceBufferMemory, nullptr);
	}

	Application::createBuffer(instancesSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, asInstanceBuffer, asInstanceBufferMemory);

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Application::createBuffer(instancesSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(
		Application::device, stagingBufferMemory, 0, instancesSize, 0, &data);
	memcpy(data, instances.data(), (size_t)instancesSize);
	vkUnmapMemory(Application::device, stagingBufferMemory);

	Application::copyBuffer(stagingBuffer, asInstanceBuffer, instancesSize);

	vkDestroyBuffer(Application::device, stagingBuffer, nullptr);
	vkFreeMemory(Application::device, stagingBufferMemory, nullptr);
}
