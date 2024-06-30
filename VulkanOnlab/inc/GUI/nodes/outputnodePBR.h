#pragma once
#include "imnodes.h"
#include <glm/glm.hpp>
#include "GUI/nodes/outputnode.h"

class OutputNodePBR :public OutputNode{
public:
	OutputNodePBR(Material* material);
	std::string getShaderCodeUniforms();
	void draw();
private:
	glm::vec3 albedoColor = glm::vec3(1.0f);
	float metallicness = 0.0f;
	float roughness = 0.0f;
};