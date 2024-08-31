#include "vulkan/object/node.h"

int RenderNode::rollingId = 0;

glm::mat4 RenderNode::getModelMatrix()
{
	glm::mat4 ret = glm::mat4(1.0f);

	for (Transformation* transformation : transformations) {
		ret *= transformation->getMatrix();
	}

	return parent ? parent->getModelMatrix() : glm::mat4(1.0f) * ret;
}

void RenderNode::addTransform(Transformation* transform)
{
    transformations.push_back(transform);
}

void RenderNode::removeTransform(Transformation* transform)
{
	for (int i = 0; i < transformations.size(); i++) {
		if (*transform == *(transformations[i])) {
			delete transformations[i];
			transformations.erase(transformations.begin() + i);
		}
	}
}

std::vector<Transformation*>& RenderNode::getTransformations()
{
	return transformations;
}

void RenderNode::addChild(RenderNode* node)
{
	children.push_back(node);
	node->setParent(this);
}

RenderNode::~RenderNode()
{
	for (int i = 0; i < transformations.size(); i++) {
		delete transformations[i];
	}
}

void RenderNode::setParent(RenderNode* node)
{
	parent = node;
}
