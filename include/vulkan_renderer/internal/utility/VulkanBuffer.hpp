#pragma once
#include "utility/Defines.hpp"
#ifdef USING_VULKAN

#include <vulkan\vulkan.h>

#include "vulkan_renderer/internal/utility/VulkanLogicalDevice.hpp"
#include "vulkan_renderer/internal/utility/vk_mem_alloc.h"

namespace Common {

	class VulkanBuffer
	{
	public:
		VulkanBuffer(VulkanLogicalDevice* device, VmaAllocator allocator, uint32_t size, VkBufferUsageFlags usage, bool gpu, VkCommandPool pool);
		VulkanBuffer(VulkanLogicalDevice* device, VmaAllocator allocator, uint32_t size, VkBufferUsageFlags usage, VmaMemoryUsage memoryType, VkCommandPool pool);
		~VulkanBuffer();

		void CreateBufferView(uint32_t range, uint32_t offset, VkFormat format);

		VkBuffer GetBuffer();

		VkBufferView GetBufferView();

		VmaAllocationInfo getAllocationInfo();

		void UpdateBuffer(void* data, uint32_t offset, uint32_t size);

		void MapBuffer(void* location);

		void UnmapBuffer();

		void ClearBuffer();

	protected:
		VmaAllocator allocator;
		VmaAllocation allocation;
		VmaAllocationInfo allocationInfo;

		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation;
		VmaAllocationInfo stagingBufferAllocationInfo;

		VkBuffer buffer;

		VkBufferView view;

		VkBufferUsageFlags usage;

		uint32_t size;

		VulkanLogicalDevice* device;

		VkCommandPool pool;

		bool gpu;


	};

}

#endif // USING_VULKAN