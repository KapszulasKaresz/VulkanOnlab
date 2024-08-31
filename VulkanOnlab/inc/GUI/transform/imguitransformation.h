#pragma once
#include <string>
#include <imgui.h>
#include "vulkan/transform/transformation.h"
#include "vulkan/transform/translation.h"
#include "vulkan/transform/rotation.h"
#include "vulkan/transform/scale.h"
#include "vulkan/transform/matrixtransform.h"

struct ImGuiObject;

struct ImGuiTransformation {
	static void draw(Transformation* transformation, ImGuiObject* object);
	static void draw(Translation* translation, ImGuiObject* object);
	static void draw(Scale* scale, ImGuiObject* object);
	static void draw(Rotation* rotation, ImGuiObject* object);
	static void draw(MatrixTransform* matrixTransform, ImGuiObject* object);
	static void drawDelete(Transformation* transformation, ImGuiObject* object);
};