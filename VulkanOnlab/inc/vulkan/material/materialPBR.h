#pragma once
#include "vulkan/material/material.h"
#include <glm/glm.hpp>

struct MaterialPBR : public Material {
	MaterialPBR();

	struct PBRUniformObject {
		alignas(16)glm::vec3 albedo;
		alignas(4)float metallic;
		alignas(4)float roughness;
	} material;

	void updateUniformBuffer(uint32_t currentImage) override;

	void createUniformBuffers() override;

	void createDescriptorSetLayout() override;
	void createDescriptorSets() override;

	~MaterialPBR();
};