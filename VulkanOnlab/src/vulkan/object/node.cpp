#include "vulkan/object/node.h"
#include "vulkan/object/object.h"

int RenderNode::rollingId = 0;

glm::mat4 RenderNode::getModelMatrix()
{
	glm::mat4 ret = glm::mat4(1.0f);

	for (Transformation* transformation : transformations) {
		ret = transformation->getMatrix() * ret;
	}

	return (parent ? parent->getModelMatrix() : glm::mat4(1.0f)) * ret;
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

std::vector<RenderNode*>& RenderNode::getChildren()
{
	return children;
}

RenderNode* RenderNode::getRootNode()
{
	return parent ? parent->getRootNode() : this;
}

RenderNode* RenderNode::getParentNode()
{
	return parent;
}

std::vector<Object*> RenderNode::getObjects()
{
	std::vector<Object*> ret;

	for (int i = 0; i < children.size(); i++) {
		if (dynamic_cast<Object*>(children[i]) != nullptr) {
			ret.push_back(dynamic_cast<Object*>(children[i]));
		}
		auto childObjects = children[i]->getObjects();
		ret.insert(ret.end(), childObjects.begin(), childObjects.end());
	}

	return ret;
}

void RenderNode::addChild(RenderNode* node)
{
	if (node->gltfID == -1) {
		children.push_back(node);
		node->setParent(this);
	}
	else {
		for (auto childNode : children) {
			if (childNode->gltfID == node->gltfID) {
				return;
			}
		}
		children.push_back(node);
		node->setParent(this);
	}
}

void RenderNode::removeSubgraph(RenderNode* node)
{
	for (int i = 0; i < children.size(); i++) {
		if (children[i]->id == node->id) {
			delete children[i];
			children.erase(children.begin() + i);
			return;
		}
	}
}

RenderNode::~RenderNode()
{
	for (int i = 0; i < children.size(); i++) {
		delete children[i];
	}
	for (int i = 0; i < transformations.size(); i++) {
		delete transformations[i];
	}
}

void RenderNode::setParent(RenderNode* node)
{
	parent = node;
}

