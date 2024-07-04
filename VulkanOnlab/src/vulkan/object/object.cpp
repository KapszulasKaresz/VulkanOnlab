#include "vulkan/object/object.h"
#include "vulkan/application.h"

VkDescriptorSetLayout Object::descriptorSetLayout = VK_NULL_HANDLE;

int Object::rollingId = 0;

glm::mat4 Object::getModelMatrix()
{
    glm::mat4 ret = glm::mat4(1.0f);

    for (Transformation* transformation : transformations) {
        ret *= transformation->getMatrix();
    }

    return ret;
}

void Object::create(const char* meshFilename)
{
	createDescriptorSetLayout();
	createMaterial();
	mesh = new Mesh();
	mesh->load(meshFilename);
	createVertexBuffer();
	createIndexBuffer();
	createUniformBuffers(); 
	createDescriptorSets(); 
}


void Object::createVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(mesh->vertices[0]) * mesh->vertices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(
		Application::device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, mesh->vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(Application::device, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBuffer, vertexBufferMemory);

	copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

	vkDestroyBuffer(Application::device, stagingBuffer, nullptr);
	vkFreeMemory(Application::device, stagingBufferMemory, nullptr);

}

uint32_t Object::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(Application::physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

void Object::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{

	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(Application::device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to create buffer!");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(Application::device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(Application::device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate buffer memory!");
	}

	vkBindBufferMemory(Application::device, buffer, bufferMemory, 0);
}

void Object::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = Application::commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(Application::device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0; // Optional 
	copyRegion.dstOffset = 0; // Optional  
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(Application::graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(Application::graphicsQueue);

	vkFreeCommandBuffers(Application::device, Application::commandPool, 1, &commandBuffer);
}

void Object::createIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(mesh->indices[0]) * mesh->indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(Application::device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, mesh->indices.data(), (size_t)bufferSize);
	vkUnmapMemory(Application::device, stagingBufferMemory);

	createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	copyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(Application::device, stagingBuffer, nullptr);
	vkFreeMemory(Application::device, stagingBufferMemory, nullptr);
}

void Object::createDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
	uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

	std::array<VkDescriptorSetLayoutBinding, 1> bindings = { uboLayoutBinding };

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings = bindings.data();

	if (vkCreateDescriptorSetLayout(Application::device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create descriptor set layout!");
	}
}

void Object::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(ObjectUniformBufferObject);

	uniformBuffers.resize(Application::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(Application::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(Application::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < Application::MAX_FRAMES_IN_FLIGHT; i++) {
		createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers[i], uniformBuffersMemory[i]);

		vkMapMemory(Application::device, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
	}
}


void Object::updateUniformBuffer(uint32_t currentImage, Camera& cam, std::vector<Light*>& lights)
{
	ObjectUniformBufferObject ubo{};
	ubo.model = getModelMatrix();
	ubo.modelInverse = glm::inverse(getModelMatrix());
	

	memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
	
	material->updateUniformBuffer(currentImage);
}


void Object::createDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(Application::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = Application::descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(Application::MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(Application::MAX_FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(Application::device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (size_t i = 0; i < Application::MAX_FRAMES_IN_FLIGHT; i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(ObjectUniformBufferObject);

		std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(Application::device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void Object::createMaterial()
{
	material = new Material();
}


void Object::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->graphicsPipeline);
		VkBuffer vertexBuffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->graphicsPipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(Application::swapChainExtent.width);
		viewport.height = static_cast<float>(Application::swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = Application::swapChainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		std::array<VkDescriptorSet, 3> descSets {Application::globalDescriptorSets[currentFrame],material->descriptorSets[currentFrame], descriptorSets[currentFrame]};

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->pipelineLayout, 0, descSets.size(), descSets.data(), 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(mesh->indices.size()), 1, 0, 0, 0);
}

Object::~Object()
{
	if(mesh != nullptr)
		delete mesh;
	

	for (int i = 0; i < transformations.size(); i++) {
		delete transformations[i];
	}
}

void Object::cleanup()
{
	if (material != nullptr)
		delete material; 

	for (size_t i = 0; i < Application::MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(Application::device, uniformBuffers[i], nullptr);
		vkFreeMemory(Application::device, uniformBuffersMemory[i], nullptr);
	}

	vkDestroyDescriptorSetLayout(Application::device, descriptorSetLayout, nullptr);

	vkDestroyBuffer(Application::device, indexBuffer, nullptr);
	vkFreeMemory(Application::device, indexBufferMemory, nullptr);

	vkDestroyBuffer(Application::device, vertexBuffer, nullptr);
	vkFreeMemory(Application::device, vertexBufferMemory, nullptr);
}
