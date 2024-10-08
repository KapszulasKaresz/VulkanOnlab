#include "vulkan/material/materialPhong.h"
#include "vulkan/texture/texture.h"
#include "vulkan/application.h"

MaterialPhong::MaterialPhong() : Material()
{
	createDescriptorSetLayout();
	createUniformBuffers();
	createDescriptorSets();
	createGraphicsPipeline("res/shaders/frag.spv");

	material.ka = glm::vec3(1.0f, 0.0f, 1.0f);
	material.kd = glm::vec3(1.0f, 0.0f, 1.0f);
	material.ks = glm::vec3(0.3f, 0.3f, 0.3f);
	material.shininess = 15.0f;
}

void MaterialPhong::updateUniformBuffer(uint32_t currentImage)
{
	memcpy(this->uniformBuffersMapped[currentImage], &(this->material), sizeof(this->material));
}

void MaterialPhong::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(PhongUniformObject);

	uniformBuffers.resize(Application::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(Application::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(Application::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < Application::MAX_FRAMES_IN_FLIGHT; i++) {
		Application::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i], uniformBuffersMemory[i]);

		vkMapMemory(Application::device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}
}

void MaterialPhong::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding materialLayoutBinding{};
	materialLayoutBinding.binding = 0;
	materialLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	materialLayoutBinding.descriptorCount = 1;
	materialLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	std::vector<VkDescriptorSetLayoutBinding> bindings;
	bindings.push_back(materialLayoutBinding);

	for (int i = 0; i < textures.size(); i++) {
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
		samplerLayoutBinding.binding = i + 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bindings.push_back(samplerLayoutBinding);
	}



	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(Application::device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void MaterialPhong::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(Application::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = Application::descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(Application::MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(Application::MAX_FRAMES_IN_FLIGHT);
	VkResult res;
	if ((res = vkAllocateDescriptorSets(Application::device, &allocInfo, descriptorSets.data())) != VK_SUCCESS) {
		assert(false);
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < Application::MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(PhongUniformObject);

		std::vector<VkWriteDescriptorSet> descriptorWrites;

		VkWriteDescriptorSet materialWrite = {};

		materialWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		materialWrite.dstSet = descriptorSets[i];
		materialWrite.dstBinding = 0;
		materialWrite.dstArrayElement = 0;
		materialWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		materialWrite.descriptorCount = 1;
		materialWrite.pBufferInfo = &bufferInfo;

		descriptorWrites.push_back(materialWrite);

		for (int j = 0; j < textures.size(); j++) {

			VkDescriptorImageInfo* imageInfo = new VkDescriptorImageInfo();
			imageInfo->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo->imageView = textures[j]->getTextureImageView();
			imageInfo->sampler = textures[j]->getTextureSampler();

			VkWriteDescriptorSet imageWrite{};

			imageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			imageWrite.dstSet = descriptorSets[i];
			imageWrite.dstBinding = j + 1;
			imageWrite.dstArrayElement = 0;
			imageWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			imageWrite.descriptorCount = 1;
			imageWrite.pImageInfo = imageInfo;

			descriptorWrites.push_back(imageWrite);
		}

		vkUpdateDescriptorSets(Application::device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

		for (int i = 1; i < descriptorWrites.size(); i++) {
			delete descriptorWrites[i].pImageInfo;
		}
	}
}

MaterialPhong::~MaterialPhong()
{
	vkDeviceWaitIdle(Application::device);
	for (size_t i = 0; i < Application::MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(Application::device, uniformBuffers[i], nullptr);
		vkFreeMemory(Application::device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorSetLayout(Application::device, descriptorSetLayout, nullptr);

	vkDestroyPipeline(Application::device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(Application::device, pipelineLayout, nullptr);
}
