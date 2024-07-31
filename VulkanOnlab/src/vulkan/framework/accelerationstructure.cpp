#include "vulkan/framework/accelerationstructure.h"
#include "vulkan/application.h"
#include <vulkan/vulkan.h>

AccelerationStructure::AccelerationStructure(VkDevice& device, VkAccelerationStructureTypeKHR type) :device(device), type(type)
{
}

VkAccelerationStructureKHR AccelerationStructure::getHandle() const
{
	return handle;
}

uint64_t AccelerationStructure::addTriangleGeometry(VkBuffer& vertexBuffer, VkBuffer& indexBuffer, VkBuffer& transformBuffer, uint32_t triangleCount, uint32_t maxVertex, VkDeviceSize vertexStride, uint32_t transformOffset, VkFormat vertexFormat, VkGeometryFlagsKHR flags)
{
	VkAccelerationStructureGeometryKHR geometry{};
	geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
	geometry.flags = flags;
	geometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
	geometry.geometry.triangles.vertexFormat = vertexFormat;
	geometry.geometry.triangles.maxVertex = maxVertex;
	geometry.geometry.triangles.vertexStride = vertexStride;
	geometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
	geometry.geometry.triangles.vertexData.deviceAddress = Application::getBufferDeviceAddress(vertexBuffer);
	geometry.geometry.triangles.indexData.deviceAddress = Application::getBufferDeviceAddress(indexBuffer);
	geometry.geometry.triangles.transformData.deviceAddress = Application::getBufferDeviceAddress(transformBuffer);

	uint64_t index = geometries.size();
	geometries.insert({ index, {geometry, triangleCount, transformOffset} });
	return index;
}

void AccelerationStructure::updateTriangleGeometry(uint64_t geometryID, VkBuffer& vertexBuffer, VkBuffer& indexBuffer, VkBuffer& transformBuffer, uint32_t triangleCount, uint32_t maxVertex, VkDeviceSize vertexStride, uint32_t transformOffset, VkFormat vertexFormat, VkGeometryFlagsKHR flags)
{
	VkAccelerationStructureGeometryKHR* geometry = &geometries[geometryID].geometry;
	geometry->sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	geometry->geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
	geometry->flags = flags;
	geometry->geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
	geometry->geometry.triangles.vertexFormat = vertexFormat;
	geometry->geometry.triangles.maxVertex = maxVertex;
	geometry->geometry.triangles.vertexStride = vertexStride;
	geometry->geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
	geometry->geometry.triangles.vertexData.deviceAddress = Application::getBufferDeviceAddress(vertexBuffer);
	geometry->geometry.triangles.indexData.deviceAddress = Application::getBufferDeviceAddress(indexBuffer);
	geometry->geometry.triangles.transformData.deviceAddress = Application::getBufferDeviceAddress(transformBuffer);
	geometries[geometryID].primitiveCount = triangleCount;
	geometries[geometryID].transformOffset = transformOffset;
	geometries[geometryID].updated = true;
}

uint64_t AccelerationStructure::addInstanceGeometry(VkBuffer& instanceBuffer, uint32_t instanceCount, uint32_t transformOffset, VkGeometryFlagsKHR flags)
{
	VkAccelerationStructureGeometryKHR geometry{};
	geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	geometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
	geometry.flags = flags;
	geometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
	geometry.geometry.instances.arrayOfPointers = VK_FALSE;
	geometry.geometry.instances.data.deviceAddress = Application::getBufferDeviceAddress(instanceBuffer);

	uint64_t index = geometries.size();
	geometries.insert({ index, {geometry, instanceCount, transformOffset} });
	return index;
}

void AccelerationStructure::updateInstanceGeometry(uint64_t geometryID, VkBuffer& instanceBuffer, uint32_t instanceCount, uint32_t transformOffset, VkGeometryFlagsKHR flags)
{
	VkAccelerationStructureGeometryKHR* geometry = &geometries[geometryID].geometry;
	geometry->sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	geometry->geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
	geometry->flags = flags;
	geometry->geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
	geometry->geometry.instances.arrayOfPointers = VK_FALSE;
	geometry->geometry.instances.data.deviceAddress = Application::getBufferDeviceAddress(instanceBuffer);
	geometries[geometryID].primitiveCount = instanceCount;
	geometries[geometryID].transformOffset = transformOffset;
	geometries[geometryID].updated = true;
}

void AccelerationStructure::build(VkBuildAccelerationStructureFlagsKHR flags, VkBuildAccelerationStructureModeKHR mode)
{
	assert(!geometries.empty());

	std::vector<VkAccelerationStructureGeometryKHR>       accelerationStructureGeometries;
	std::vector<VkAccelerationStructureBuildRangeInfoKHR> accelerationStructureBuildRangeInfos;
	std::vector<uint32_t>                                 primitiveCounts;

	for (auto& geometry : geometries)
	{
		if (mode == VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR && !geometry.second.updated)
		{
			continue;
		}
		accelerationStructureGeometries.push_back(geometry.second.geometry);

		VkAccelerationStructureBuildRangeInfoKHR buildRangeInfo;
		buildRangeInfo.primitiveCount = geometry.second.primitiveCount;
		buildRangeInfo.primitiveOffset = 0;
		buildRangeInfo.firstVertex = 0;
		buildRangeInfo.transformOffset = geometry.second.transformOffset;
		accelerationStructureBuildRangeInfos.push_back(buildRangeInfo);
		primitiveCounts.push_back(geometry.second.primitiveCount);
		geometry.second.updated = false;
	}

	VkAccelerationStructureBuildGeometryInfoKHR buildGeometryInfo{};
	buildGeometryInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	buildGeometryInfo.type = type;
	buildGeometryInfo.flags = flags;
	buildGeometryInfo.mode = mode;
	if (mode == VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR && handle != VK_NULL_HANDLE)
	{
		buildGeometryInfo.srcAccelerationStructure = handle;
		buildGeometryInfo.dstAccelerationStructure = handle;
	}
	buildGeometryInfo.geometryCount = static_cast<uint32_t>(accelerationStructureGeometries.size());
	buildGeometryInfo.pGeometries = accelerationStructureGeometries.data();

	// Get required build sizes
	buildSizesInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
	Application::ProcAddress::vkGetAccelerationStructureBuildSizesKHR(
		device,
		VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
		&buildGeometryInfo,
		primitiveCounts.data(),
		&buildSizesInfo);

	// Create a buffer for the acceleration structure
	if (buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(device, buffer, nullptr);
		vkFreeMemory(device, bufferMemory, nullptr);
	}
	Application::createBuffer(buildSizesInfo.accelerationStructureSize, VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT , buffer, bufferMemory);
	VkAccelerationStructureCreateInfoKHR accelerationStructureCreateInfo{};
	accelerationStructureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
	accelerationStructureCreateInfo.buffer = buffer;
	accelerationStructureCreateInfo.size = buildSizesInfo.accelerationStructureSize;
	accelerationStructureCreateInfo.type = type;

	if (Application::ProcAddress::vkCreateAccelerationStructureKHR(device, &accelerationStructureCreateInfo, nullptr, &handle) != VK_SUCCESS)
	{
		throw std::runtime_error{ "Could not create acceleration structure" };
	}

	// Get the acceleration structure's handle
	VkAccelerationStructureDeviceAddressInfoKHR accelerationDeviceAddressInfo{};
	accelerationDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
	accelerationDeviceAddressInfo.accelerationStructure = handle;
	deviceAddress = Application::ProcAddress::vkGetAccelerationStructureDeviceAddressKHR(device, &accelerationDeviceAddressInfo);

	Application::createBuffer(buildSizesInfo.buildScratchSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT , scratchBuffer, scratchBufferMemory);

	buildGeometryInfo.scratchData.deviceAddress = Application::getBufferDeviceAddress(scratchBuffer);
	buildGeometryInfo.dstAccelerationStructure = handle;

	// Build the acceleration structure on the device via a one - time command buffer submission
	auto commandBuffer = Application::beginSingleTimeCommands();
	auto asBuildRangeInfos = &*accelerationStructureBuildRangeInfos.data();

	Application::ProcAddress::vkCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &buildGeometryInfo, &asBuildRangeInfos);
	Application::endSingleTimeCommands(commandBuffer);

	vkDestroyBuffer(device, scratchBuffer, nullptr); 
	vkFreeMemory(device, scratchBufferMemory, nullptr);
	scratchBuffer = VK_NULL_HANDLE; 
}

uint64_t AccelerationStructure::getDeviceAddress() const
{
	return deviceAddress;
}

VkBuffer AccelerationStructure::getBuffer()
{
	return buffer;
}

void AccelerationStructure::resetGeometries()
{
	geometries.clear();
}

bool AccelerationStructure::isEmpty()
{
	return geometries.empty();
}

AccelerationStructure::~AccelerationStructure()
{
	vkDeviceWaitIdle(device);
	if (VK_NULL_HANDLE != handle) {
		Application::ProcAddress::vkDestroyAccelerationStructureKHR(device, handle, nullptr);
	}
	vkDestroyBuffer(device, buffer, nullptr);
	vkFreeMemory(device, bufferMemory, nullptr);
}
