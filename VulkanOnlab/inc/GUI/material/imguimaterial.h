#pragma once
#include "vulkan/material/material.h"
#include "GUI/nodes/nodeeditor.h"

class ImGuiMaterial {
public:
	void draw(Material* material, NodeEditor* nodeEditor);
};