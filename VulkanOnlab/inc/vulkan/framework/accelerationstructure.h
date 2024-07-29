#pragma once
#include <vulkan/vulkan.h>
#include <map>

class AccelerationStructure {
public:
	AccelerationStructure(VkDevice& device, VkAccelerationStructureTypeKHR type);

	VkAccelerationStructureKHR getHandle() const;

	uint64_t addTriangleGeometry(VkBuffer& vertexBuffer,
		VkBuffer& indexBuffer,
		VkBuffer& transformBuffer,
		uint32_t                             triangleCount,
		uint32_t                             maxVertex,
		VkDeviceSize                         vertexStride,
		uint32_t                             transformOffset = 0,
		VkFormat                             vertexFormat = VK_FORMAT_R32G32B32_SFLOAT,
		VkGeometryFlagsKHR                   flags = VK_GEOMETRY_OPAQUE_BIT_KHR);

	void updateTriangleGeometry(uint64_t geometryID,
		VkBuffer& vertexBuffer,
		VkBuffer& indexBuffer,
		VkBuffer& transformBuffer,
		uint32_t                             triangleCount,
		uint32_t                             maxVertex,
		VkDeviceSize                         vertexStride,
		uint32_t                             transformOffset = 0,
		VkFormat                             vertexFormat = VK_FORMAT_R32G32B32_SFLOAT,
		VkGeometryFlagsKHR                   flags = VK_GEOMETRY_OPAQUE_BIT_KHR);

	uint64_t addInstanceGeometry(VkBuffer& instanceBuffer,
		uint32_t                            instanceCount,
		uint32_t                            transformOffset = 0,
		VkGeometryFlagsKHR                  flags = VK_GEOMETRY_OPAQUE_BIT_KHR);

	void updateInstanceGeometry(uint64_t geometryID,
		VkBuffer& instanceBuffer,
		uint32_t                            instanceCount,
		uint32_t                            transformOffset = 0,
		VkGeometryFlagsKHR                  flags = VK_GEOMETRY_OPAQUE_BIT_KHR);

	void build(VkQueue                              queue,
		VkBuildAccelerationStructureFlagsKHR flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR,
		VkBuildAccelerationStructureModeKHR  mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR);

	uint64_t getDeviceAddress() const;

	VkBuffer getBuffer();

	void resetGeometries();

	~AccelerationStructure();
private:
	VkDevice& device;

	VkAccelerationStructureTypeKHR type{};
	VkAccelerationStructureKHR handle{ VK_NULL_HANDLE };

	uint64_t deviceAddress{ 0 };

	VkAccelerationStructureBuildSizesInfoKHR buildSizesInfo{};

	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory bufferMemory;

	VkBuffer scratchBuffer = VK_NULL_HANDLE;
	VkDeviceMemory scratchBufferMemory;

	struct Geometry
	{
		VkAccelerationStructureGeometryKHR geometry{};
		uint32_t                           primitiveCount{};
		uint32_t                           transformOffset{};
		bool                               updated = false;
	};

	std::map<uint64_t, Geometry> geometries{};
};