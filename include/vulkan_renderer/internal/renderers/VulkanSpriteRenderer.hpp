#pragma once
#include "utility/Defines.hpp"
#ifdef USING_VULKAN

#include <vulkan/vulkan.h>

#include "vulkan_renderer/internal/utility/VulkanPipeline.hpp"
#include "vulkan_renderer/internal/utility/VulkanLogicalDevice.hpp"
#include "vulkan_renderer/internal/utility/VulkanBuffer.hpp"
#include "vulkan_renderer/internal/utility/VulkanDescriptorPool.hpp"
#include "glm/glm.hpp"

namespace Common {

	class VulkanRenderer;

	class VulkanSpriteRenderer
	{
	public:
		VulkanSpriteRenderer(VulkanRenderer* renderer, VulkanLogicalDevice* device, VulkanDescriptorPool* descriptorPool);
		~VulkanSpriteRenderer();

		void StartRender(VkCommandBuffer renderPassBuffer, glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

		void RenderSprite(std::weak_ptr<VulkanTexture> texture, glm::mat4 modelMatrix);

		void FinishRender();

		void Clean();

		void Recreate();

	protected:
		std::unique_ptr<VulkanPipeline> renderPassPipeline;

		std::unique_ptr<VulkanBuffer> uboBuffer;
		std::unique_ptr<VulkanBuffer> vertexBuffer;
		std::unique_ptr<VulkanBuffer> indexBuffer;

		std::vector<uint32_t> indices;
		
		typedef struct {
			glm::vec3 position;
			glm::vec2 texCoord;
		}VertexInfo_t;

		typedef struct {
			glm::mat4 view;
			glm::mat4 proj;
		}Ubo_t;

		Ubo_t ubo;

		typedef struct {
			glm::mat4 model;
		}PushConstants_t;

		VulkanRenderer* renderer;
		VulkanLogicalDevice* device;
		VulkanDescriptorPool* descriptorPool;

		VkDescriptorSet renderUboDescriptorSet;

		VkCommandBuffer renderCommandBuffer;

		
	};

}

#endif // USING_VULKAN