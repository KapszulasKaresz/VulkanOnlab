#include "vulkan/object/object.h"
#include "vulkan/application.h"
#include "vulkan/material/materialPhong.h"
#include "vulkan/material/materialstore.h"


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
	if (descriptorSetLayout == VK_NULL_HANDLE) {
		createDescriptorSetLayout();
	}
	createMaterial();
	mesh = new Mesh();
	mesh->load(meshFilename);
	createBottomLevelAccelerationStructure();
	createUniformBuffers(); 
	createDescriptorSets(); 
}

void Object::create(tinygltf::Primitive* primitve, tinygltf::Model* gltfModel)
{
	if (descriptorSetLayout == VK_NULL_HANDLE) {
		createDescriptorSetLayout();
	}
	createMaterial();
	mesh = new Mesh();
	mesh->load(primitve, gltfModel);
	createBottomLevelAccelerationStructure();
	createUniformBuffers();
	createDescriptorSets();
}

void Object::swapMaterial(Material* material)
{
	if (this->material != nullptr) {
		this->material->removeObject(this);
	}
	this->material = material;
	material->addObject(this);
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

void Object::checkTransformationUpdate()
{
	if (oldTransform != getModelMatrix() && hasAccelerationStructure) {
		oldTransform = getModelMatrix();
		updateBottomLevelAccelerationStructureTransform();
		accelerationStructureDirty = true;
	}
}

void Object::createUniformBuffers()
{
	VkDeviceSize bufferSize = sizeof(ObjectUniformBufferObject);

	uniformBuffers.resize(Application::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMemory.resize(Application::MAX_FRAMES_IN_FLIGHT);
	uniformBuffersMapped.resize(Application::MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < Application::MAX_FRAMES_IN_FLIGHT; i++) {
		Application::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
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
	this->swapMaterial(MaterialStore::defaultMaterial);
}


void Object::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t currentFrame)
{
	
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->graphicsPipeline);
		VkBuffer vertexBuffers[] = { mesh->vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

		vkCmdBindIndexBuffer(commandBuffer, mesh->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

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

bool Object::getShadowCast()
{
	return castShadow;
}

void Object::toggleShadowCast()
{
	castShadow = !castShadow;
	if (castShadow) {
		instance.mask = CAST_SHADOW; 
	}
	else {
		instance.mask = NO_CAST_SHADOW;
	}

	accelerationStructureDirty = true;
}

void Object::createBottomLevelAccelerationStructure()
{
	transformMatrixBuffer;
	transformMatrixBufferMemory;

	Application::createBuffer(sizeof(glm::mat4), VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT , transformMatrixBuffer, transformMatrixBufferMemory);
	

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	Application::createBuffer(sizeof(glm::mat4), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	glm::mat4 modelMatrix = glm::mat4(1.0);

	void* data;
	vkMapMemory(
		Application::device, stagingBufferMemory, 0, sizeof(glm::mat4), 0, &data);
	memcpy(data, &modelMatrix, (size_t)sizeof(glm::mat4));
	vkUnmapMemory(Application::device, stagingBufferMemory);

	Application::copyBuffer(stagingBuffer, transformMatrixBuffer, sizeof(glm::mat4));

	vkDestroyBuffer(Application::device, stagingBuffer, nullptr);
	vkFreeMemory(Application::device, stagingBufferMemory, nullptr);

	if (bottomLevelAS == nullptr) {
		bottomLevelAS = new AccelerationStructure(Application::device, VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR);
		bottomLevelAS->addTriangleGeometry(
							mesh->vertexBuffer,
							mesh->indexBuffer,
							transformMatrixBuffer,
							static_cast<uint32_t>(mesh->indices.size()) / 3,
							static_cast<uint32_t>(mesh->vertices.size()),
							sizeof(Vertex),
							0, VK_FORMAT_R32G32B32_SFLOAT, VK_GEOMETRY_OPAQUE_BIT_KHR
							);
	}
	bottomLevelAS->build(VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR, VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR);


	VkTransformMatrixKHR transformMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f };

	instance = {};
	instance.transform = transformMatrix;
	instance.instanceCustomIndex = 0;
	instance.mask = CAST_SHADOW;
	instance.instanceShaderBindingTableRecordOffset = 0;
	instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
	instance.accelerationStructureReference = bottomLevelAS->getDeviceAddress();

	accelerationStructureDirty = true;
	hasAccelerationStructure = true;
}

VkTransformMatrixKHR Object::convertToVkTransformMatrixKHR(const glm::mat4& mat)
{
	VkTransformMatrixKHR transformMatrixKHR;

	for (int row = 0; row < 3; ++row) {
		for (int col = 0; col < 4; ++col) {
			transformMatrixKHR.matrix[row][col] = mat[col][row];
		}
	}

	return transformMatrixKHR;
}

void Object::updateBottomLevelAccelerationStructureTransform()
{
	instance.transform = convertToVkTransformMatrixKHR(getModelMatrix());
}

void Object::cleanup()
{
	if (material != nullptr) {
		material->removeObject(this);
	}

	if (bottomLevelAS != nullptr) {
		delete bottomLevelAS;
	}

	if (transformMatrixBuffer != VK_NULL_HANDLE) {
		vkDestroyBuffer(Application::device, transformMatrixBuffer, nullptr);
		vkFreeMemory(Application::device, transformMatrixBufferMemory, nullptr);
	}

	for (size_t i = 0; i < Application::MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroyBuffer(Application::device, uniformBuffers[i], nullptr);
		vkFreeMemory(Application::device, uniformBuffersMemory[i], nullptr);
	}
}
