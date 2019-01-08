#include "vulkan_renderer/internal/utility/VulkanLogicalDevice.hpp"
#ifdef USING_VULKAN

#include <set>
#include <string>
#include <iostream>

namespace Common {
	

	VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice * vulkanPhysicalDevice, bool debug, std::vector<const char*> extensions, VulkanDeviceFeatures_t requestedDeviceFeatures)
	{
		std::vector<VkDeviceQueueCreateInfo> queues;
		std::set<int> uniqueQueueFamilies = { vulkanPhysicalDevice->GetQueueFamilies().graphics, 
			vulkanPhysicalDevice->GetQueueFamilies().compute, 
			vulkanPhysicalDevice->GetQueueFamilies().present };
		float queuePriority = 1.f;

		for (int queueFamily : uniqueQueueFamilies) {
			VkDeviceQueueCreateInfo QueueCreateInfo = {};
			QueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			QueueCreateInfo.queueFamilyIndex = queueFamily;
			QueueCreateInfo.queueCount = 1;

			QueueCreateInfo.pQueuePriorities = &queuePriority;

			queues.push_back(QueueCreateInfo);

		}

		VkBool32 physicalDeviceFeatures[sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32)];
		float* requestedFeatures = reinterpret_cast<float*>(&requestedDeviceFeatures);

		for (int i = 0; i < sizeof(VkPhysicalDeviceFeatures) / sizeof(VkBool32); ++i) {
			if (requestedFeatures[i] > 0.f)
				physicalDeviceFeatures[i] = VK_TRUE;
			else
				physicalDeviceFeatures[i] = VK_FALSE;
		}

		VkPhysicalDeviceFeatures features = {};
		memcpy(&features, &physicalDeviceFeatures, sizeof(VkPhysicalDeviceFeatures));

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queues.size());
		createInfo.pQueueCreateInfos = queues.data();

		createInfo.pEnabledFeatures = &features;

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		std::vector<const char*> layers = {
			"VK_LAYER_LUNARG_standard_validation"
		};

		if (debug) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
			createInfo.ppEnabledLayerNames = layers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		
		VkResult res = vkCreateDevice(vulkanPhysicalDevice->GetPhysicalDevice(), &createInfo, nullptr, &device);

		if (res != VK_SUCCESS) {
			std::string s = "[ERROR] [CODE:" + std::to_string(res) + "] Logical device creation failed";
			std::cout << s << std::endl;
		}

		vkGetDeviceQueue(device, vulkanPhysicalDevice->GetQueueFamilies().graphics, 0, &graphicsQueue);
		vkGetDeviceQueue(device, vulkanPhysicalDevice->GetQueueFamilies().compute, 0, &computeQueue);
		vkGetDeviceQueue(device, vulkanPhysicalDevice->GetQueueFamilies().present, 0, &presentQueue);

		this->vulkanPhysicalDevice = vulkanPhysicalDevice;

	}

	VulkanLogicalDevice::~VulkanLogicalDevice()
	{
		vkDestroyDevice(device, nullptr);
	}

	VkDevice VulkanLogicalDevice::GetDevice()
	{
		return device;
	}

	VulkanPhysicalDevice * VulkanLogicalDevice::getPhysicalDevice()
	{
		return vulkanPhysicalDevice;
	}

	VkQueue VulkanLogicalDevice::GetGraphicsQueue()
	{
		return graphicsQueue;
	}

	VkQueue VulkanLogicalDevice::GetComputeQueue()
	{
		return computeQueue;
	}

	VkQueue VulkanLogicalDevice::GetPresentQueue()
	{
		return presentQueue;
	}

}

#endif // USING_VULKAN