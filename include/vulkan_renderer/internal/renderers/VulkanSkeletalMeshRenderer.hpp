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

	class VulkanSkeletalMeshRenderer
	{
	public:
		VulkanSkeletalMeshRenderer(VulkanRenderer* renderer, VulkanLogicalDevice* device, VulkanDescriptorPool* descriptorPool);
		~VulkanSkeletalMeshRenderer();

		void StartRender(glm::mat4 view, glm::mat4 projection);

		void RenderMesh(const glm::mat4x4& modelMatrix, VulkanMesh* mesh,
			VulkanMaterial* material, Skeleton* skeleton,
			size_t animation,
			float time, float ticksPerSecond, float duration, bool looping,
			const glm::vec4& mainColor = glm::vec4(1.f, 1.f, 1.f, 1.f));

		void FinishRender(size_t threadID, VkCommandPool commandPool, VkCommandBuffer buffer);

		void RenderShadows(size_t threadID, VkCommandPool commandPool, VkCommandBuffer buffer, uint32_t lightOffset);

		void Clean();
		void Recreate();

		const int MAX_INSTANCE_COUNT = 256;

	protected:

		std::unique_ptr<VulkanPipeline> skeletalMeshPipeline_;
		std::unique_ptr<VulkanPipeline> shadowPipeline_;

		std::unique_ptr<VulkanBuffer> uniformBuffer_;

		typedef struct {
			glm::mat4 view;
			glm::mat4 proj;
		}Ubo_t;

		Ubo_t ubo_;

		std::shared_ptr<VulkanTexture> defaultTexture_;

		VkDescriptorSet oldBones_;

		std::vector<VkDescriptorSet> uboDescriptors_;

		typedef struct {
			glm::mat4 model;
			glm::vec4 color;
			float time;
			float ticksPerSecond;
			float duration;
			bool looping;
		}PushConstants_t;

		struct MeshData {
			glm::mat4 transform;
			VulkanMaterial* material;
			Skeleton* skeleton;
			VulkanTexture* boneData;
			VulkanMesh* mesh;
			glm::vec4 color;
			size_t animation;
			float time;
			float ticksPerSecond;
			float duration;
			bool looping;
		};

		std::vector<MeshData> meshes;

		VulkanRenderer* renderer_;
		VulkanLogicalDevice* device_;
		VulkanDescriptorPool* descriptorPool_;
		VkCommandPool commandPool_;
		VmaAllocator allocator_;

		VkCommandBuffer commandBuffer_;
	};

}

#endif // USING_VULKAN