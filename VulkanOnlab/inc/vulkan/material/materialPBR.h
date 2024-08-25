#pragma once
#include "vulkan/material/material.h"
#include <glm/glm.hpp>

class Texture2D;
class Cubemap;

struct MaterialPBR : public Material {
	MaterialPBR();

	struct PBRUniformObject {
		alignas(16)glm::vec3 albedo;
		alignas(4)float metallic;
		alignas(4)float roughness;
		alignas(4)float maxMipLevel;
	} material;

	void updateUniformBuffer(uint32_t currentImage) override;

	void createUniformBuffers() override;
	void setTexture(std::vector<Texture*> texturesin) override;

	void createDescriptorSetLayout() override;
	void createDescriptorSets() override;

	void swapBRDF(const char* filename);
	void swapSpecularMap(const char* filename);
	void swapIrradianceMap(const char* filename);

	const int NUMBER_OF_PREUSED_TEXTURES = 3;
	static Texture2D* brdfLUT;
	static Cubemap* cubeMap;
	static Cubemap* irradianceMap;
	static int PBRExistanceCount;

	bool doubleSided = false;
	~MaterialPBR();
};