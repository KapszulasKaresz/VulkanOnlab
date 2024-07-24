#pragma once
#include "vulkan/material/material.h"
#include <glm/glm.hpp>

class Texture2D;
class Cubemap;

struct MaterialEnvMap : public Material {
	MaterialEnvMap();

	void updateUniformBuffer(uint32_t currentImage) override;

	void createUniformBuffers() override;
	void setTexture(std::vector<Texture*> texturesin) override;

	void createDescriptorSetLayout() override;
	void createDescriptorSets() override;

	void swapCubeMap(const char* filename);

	const int NUMBER_OF_PREUSED_TEXTURES = 1;
	Cubemap* cubeMap;
	~MaterialEnvMap();

protected:
	void createGraphicsPipeline(const char* fragmentFileName) override;
};