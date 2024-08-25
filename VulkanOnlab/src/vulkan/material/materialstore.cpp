#include "vulkan/material/materialstore.h"
#include "vulkan/material/materialPhong.h"
#include "vulkan/material/materialPBR.h"

std::vector<std::pair<Material*, NodeEditor*>> MaterialStore::materials = {};
MaterialPhong* MaterialStore::defaultMaterial = nullptr;
 
void MaterialStore::addMaterial(Material* material)
{
	materials.push_back(std::make_pair(material, new NodeEditor(material)));
}

void MaterialStore::removeMaterial(int id)
{
	for (int i = 0; i < materials.size(); i++) {
		if (materials[i].first->id == id) {
			materials[i].first->swapMaterial(defaultMaterial);
			delete materials[i].first;
			delete materials[i].second;
			materials.erase(materials.begin() + i);
		}
	}
}

void MaterialStore::swapMaterial(Material* material)
{
	for (int i = 0; i < materials.size(); i++) {
		if (materials[i].first->id == material->id) {
			materials[i].first->swapMaterial(material);
			delete materials[i].first;
			materials[i].first = material;
		}
	}
}

void MaterialStore::clear()
{
	for (auto material : materials) {
		delete material.first;
		delete material.second;
	}
	materials.clear();
	delete defaultMaterial;
}

void MaterialStore::recreateAllPBRDescriptor()
{
	for (int i = 0; i < materials.size(); i++) {
		if (dynamic_cast<MaterialPBR*>(materials[i].first)) {
			materials[i].first->recreateDescriptors();
		}
	}
}
