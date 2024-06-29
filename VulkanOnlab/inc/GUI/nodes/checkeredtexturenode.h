#pragma once
#include "GUI/nodes/texturenode.h"
#include <glm/glm.hpp>

class CheckeredTextureNode : public TextureNode {
public:
	CheckeredTextureNode(int id, SharedGraphicsInfo graphInfo);

	void draw();

private:
	glm::vec3 color1 = glm::vec3(0.0f);
	glm::vec3 color2 = glm::vec3(1.0f); 
	float scale = 1.0f;
	bool generate = false;
};