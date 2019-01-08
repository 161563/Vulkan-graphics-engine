#pragma once
#include "utility/Defines.hpp"
#ifdef USING_VULKAN

#include <vulkan\vulkan.hpp>
#include <vector>

namespace Common {

	class VulkanLogicalDevice;

	class VulkanDescriptorPool
	{
	public:
		VulkanDescriptorPool(VulkanLogicalDevice* device);
		~VulkanDescriptorPool();

		void addPoolSize(VkDescriptorType type, uint32_t count);

		void compile(uint32_t maxSets);

		VkDescriptorPool getPool();

		void allocateDescriptorSet(uint32_t descriptorSetCount, VkDescriptorSetLayout* layouts, VkDescriptorSet* sets);

		void descriptorSetBindToBuffer(VkDescriptorSet set, VkBuffer buffer, VkDeviceSize offset,
			VkDeviceSize range, uint32_t binding, uint32_t arrayElement,
			VkDescriptorType type, uint32_t descriptorCount);

		void descriptorSetBindToImage(VkDescriptorSet set, VkImageLayout layout, VkImageView imageView, VkSampler sampler, uint32_t binding, uint32_t arrayElement, VkDescriptorType type, uint32_t descriptorCount);

	private:
		std::vector<VkDescriptorPoolSize> poolSizes;

		VkDescriptorPool pool;
		VulkanLogicalDevice* device;
	};

}

#endif // USING_VULKAN