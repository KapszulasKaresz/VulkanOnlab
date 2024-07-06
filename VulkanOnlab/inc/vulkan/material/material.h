#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <string>


class Texture;
class Object;

struct Material {
	std::vector<Texture*> textures;

	Material();

	virtual void createDescriptorSetLayout() = 0;
	virtual void createDescriptorSets() = 0;
	std::vector<VkDescriptorSet> descriptorSets;
	VkDescriptorSetLayout descriptorSetLayout;

	void recreatePipeline(const char* fragmentFileName);

	void recreateDescriptors();
	virtual void setTexture(std::vector<Texture*> texturesin);
	void addObject(Object* object);
	void removeObject(Object* object);
	void swapMaterial(Material* material);

	virtual void updateUniformBuffer(uint32_t currentImage) = 0;

	virtual void createUniformBuffers() = 0;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	int id;

	virtual ~Material();
protected:
	static int rollingId;
	std::vector<Object*> objects;
	void createGraphicsPipeline(const char* fragmentFileName);
	static std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);
};