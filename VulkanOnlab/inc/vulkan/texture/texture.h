#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

class Texture {
public:
	Texture() { id = rollingId++; }

	virtual VkImageView getTextureImageView() const = 0;
	virtual VkSampler getTextureSampler() const = 0;
	virtual void reset() = 0;
	VkDescriptorSet DS = VK_NULL_HANDLE;

	int id;
	virtual ~Texture() {}
protected:

	static int rollingId;
};
