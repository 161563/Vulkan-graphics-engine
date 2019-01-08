#pragma once
#include "utility/Defines.hpp"
#ifdef USING_VULKAN
#include <vulkan/vulkan.h>

#include "vulkan_renderer/internal/utility/VulkanPhysicalDevice.hpp"

#include <vector>

namespace Common {

	class VulkanLogicalDevice
	{
	public:
		VulkanLogicalDevice(VulkanPhysicalDevice* vulkanPhysicalDevice, bool debug, std::vector<const char*> extensions, VulkanDeviceFeatures_t requestedDeviceFeatures);
		~VulkanLogicalDevice();

		VkDevice GetDevice();

		VulkanPhysicalDevice* getPhysicalDevice();

		VkQueue GetGraphicsQueue();
		VkQueue GetComputeQueue();
		VkQueue GetPresentQueue();

	protected:
		VkDevice device;

		VulkanPhysicalDevice* vulkanPhysicalDevice;

		VkQueue graphicsQueue;
		VkQueue computeQueue;
		VkQueue presentQueue;
	};

}

#endif // USING_VULKAN