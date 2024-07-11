#pragma once
#include "vulkan/material/material.h"
#include "GUI/nodes/nodeeditor.h"
#include "GUI/material/renamewindow.h"

class ImGuiMaterial {
public:
	void draw(Material* material, NodeEditor* nodeEditor);
	RenameWindow renameWindow;
};