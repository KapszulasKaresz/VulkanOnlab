#pragma once
#include <string>
#include <imgui.h>
#include "vulkan/transform/transformation.h"
#include "vulkan/transform/translation.h"
#include "vulkan/transform/rotation.h"
#include "vulkan/transform/scale.h"
#include "vulkan/transform/matrixtransform.h"

struct ImGuiObject;
class RenderNode;

struct ImGuiTransformation {
	static void draw(Transformation* transformation, RenderNode* object);
	static void draw(Translation* translation, RenderNode* object);
	static void draw(Scale* scale, RenderNode* object);
	static void draw(Rotation* rotation, RenderNode* object);
	static void draw(MatrixTransform* matrixTransform, RenderNode* object);
	static void drawDelete(Transformation* transformation, RenderNode* object);
};