#include "vulkan_renderer/internal/utility/VulkanDescriptorPool.hpp"
#ifdef USING_VULKAN

#include <iostream>

#include "vulkan_renderer/internal/utility/VulkanLogicalDevice.hpp"

namespace Common {

	VulkanDescriptorPool::VulkanDescriptorPool(VulkanLogicalDevice* device)
	{
		this->device = device;
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		if (pool != 0)
			vkDestroyDescriptorPool(device->GetDevice(), pool, nullptr);
	}

	void VulkanDescriptorPool::addPoolSize(VkDescriptorType type, uint32_t count)
	{
		VkDescriptorPoolSize size = {};
		size.type = type;
		size.descriptorCount = count;

		poolSizes.push_back(size);
	}

	void VulkanDescriptorPool::compile(uint32_t maxSets)
	{
		VkDescriptorPoolCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		createInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		createInfo.pPoolSizes = poolSizes.data();
		createInfo.maxSets = maxSets;
		createInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;

		VkResult res = vkCreateDescriptorPool(device->GetDevice(), &createInfo, nullptr, &pool);
		if (res != VK_SUCCESS) {
			std::string s = "[ERROR] [CODE:" + std::to_string(res) + "] Failed to create descriptor pool";
			std::cout << s << std::endl;
		}
	}

	VkDescriptorPool VulkanDescriptorPool::getPool()
	{
		return pool;
	}

	void VulkanDescriptorPool::allocateDescriptorSet(uint32_t descriptorSetCount, VkDescriptorSetLayout * layouts, VkDescriptorSet * sets)
	{
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = pool;
		allocInfo.descriptorSetCount = descriptorSetCount;
		allocInfo.pSetLayouts = layouts;

		VkResult res = vkAllocateDescriptorSets(device->GetDevice(), &allocInfo, sets);
		if (res != VK_SUCCESS) {
			std::string s = "[ERROR] [CODE:" + std::to_string(res) + "] Failed to allocate descriptor sets";
			std::cout << s << std::endl;
		}
	}

	void VulkanDescriptorPool::descriptorSetBindToBuffer(VkDescriptorSet set, VkBuffer buffer, VkDeviceSize offset,
		VkDeviceSize range, uint32_t binding, uint32_t arrayElement,
		VkDescriptorType type, uint32_t descriptorCount)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = buffer;
		bufferInfo.offset = offset;
		bufferInfo.range = range;
		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = set;
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = arrayElement;
		descriptorWrite.descriptorType = type;
		descriptorWrite.descriptorCount = descriptorCount;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(device->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}

	void VulkanDescriptorPool::descriptorSetBindToImage(VkDescriptorSet set, VkImageLayout layout, VkImageView imageView, VkSampler sampler, uint32_t binding, uint32_t arrayElement, VkDescriptorType type, uint32_t descriptorCount)
	{
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageLayout = layout;
		imageInfo.imageView = imageView;
		imageInfo.sampler = sampler;

		VkWriteDescriptorSet writeInfo = {};
		writeInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeInfo.dstSet = set;
		writeInfo.dstBinding = binding;
		writeInfo.dstArrayElement = arrayElement;
		writeInfo.descriptorType = type;
		writeInfo.descriptorCount = descriptorCount;
		writeInfo.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(device->GetDevice(), 1, &writeInfo, 0, nullptr);
	}

}

#endif // USING_VULKAN