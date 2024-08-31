#pragma once
#include <glm/glm.hpp>
#include "vulkan/transform/transformation.h"
#include <vector>

class RenderNode
{
public:
	RenderNode() : id(rollingId++) {}

	glm::mat4 getModelMatrix();

	void addTransform(Transformation* transform);
	void removeTransform(Transformation* transform);

	std::vector<Transformation*>& getTransformations();

	void addChild(RenderNode* node);

	int id;

	virtual ~RenderNode();
private:
	std::vector<Transformation*> transformations;

	void setParent(RenderNode* node);
	RenderNode* parent = nullptr;
	std::vector<RenderNode*> children;

	static int rollingId;
};