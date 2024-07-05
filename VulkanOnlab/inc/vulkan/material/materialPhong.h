#pragma once
#include "vulkan/material/material.h"
#include <glm/glm.hpp>

struct MaterialPhong : public Material {
	MaterialPhong();

	struct PhongUniformObject {
		alignas(4)float shininess;
		alignas(16)glm::vec3 ks;
		alignas(16)glm::vec3 kd;
		alignas(16)glm::vec3 ka;
	} material;

	void updateUniformBuffer(uint32_t currentImage) override;

	void createUniformBuffers() override;

	void createDescriptorSetLayout() override;
	void createDescriptorSets() override;

	~MaterialPhong();
};