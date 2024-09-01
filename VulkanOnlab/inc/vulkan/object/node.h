#pragma once
#include <glm/glm.hpp>
#include "vulkan/transform/transformation.h"
#include <vector>

class Object;

class RenderNode
{
public:
	RenderNode() : id(rollingId++) {}

	std::string name;

	glm::mat4 getModelMatrix();

	void addTransform(Transformation* transform);
	void removeTransform(Transformation* transform);

	std::vector<Transformation*>& getTransformations();
	std::vector<RenderNode*>& getChildren();

	RenderNode* getRootNode();
	RenderNode* getParentNode();

	std::vector<Object*> getObjects();

	void addChild(RenderNode* node);
	void removeSubgraph(RenderNode* node);

	int id;

	int gltfID = -1;

	virtual ~RenderNode();
private:
	std::vector<Transformation*> transformations;

	void setParent(RenderNode* node);
	RenderNode* parent = nullptr;
	std::vector<RenderNode*> children;

	static int rollingId;
};