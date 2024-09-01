#pragma once
#include <imgui.h>
#include "vulkan/object/object.h"
#include "GUI/transform/imguitransformation.h"
#include "vulkan/transform/translation.h"
#include "vulkan/transform/scale.h"
#include <vector>

class Scene;
class NodeEditor;
struct MainMenu;

struct ImGuiObject {
	static void draw(RenderNode* node);

private:
	static void addChildNode(RenderNode* node);
	static void addMesh(RenderNode* node);
	static void deleteNode(RenderNode* node);
};