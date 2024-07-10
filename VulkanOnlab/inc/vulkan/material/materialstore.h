#pragma once
#include "vulkan/material/material.h"
#include "vulkan/material/materialPhong.h"
#include "GUI/nodes/nodeeditor.h"
#include <vector>
#include <utility>


class MaterialStore {
public:
	static void addMaterial(Material* material);
	static void removeMaterial(int id);
	static void swapMaterial(Material* material);

	static MaterialPhong* defaultMaterial;

	static void clear();
	static std::vector<std::pair<Material*, NodeEditor*>> materials;	
};