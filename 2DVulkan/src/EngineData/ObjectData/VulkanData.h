#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <glm.hpp>
#include <memory>

#include "EngineData/RenderData/VulkanData/Texture.h"
#include "EngineData/RenderData/VulkanData/GlyphTexture.h"
#include "Text/FontPushConstants.h"

struct VulkanData
{
	VkDescriptorSetLayout vulkanDescriptorSetLayout = VK_NULL_HANDLE;
	VkDescriptorPool vulkanDescriptorPool = VK_NULL_HANDLE;

	std::vector<VkDescriptorSet> vulkanDescriptorSets{};

	std::vector<VkBuffer> vulkanUniformBuffers{};
	std::vector<VkDeviceMemory> vulkanUniformMemory{};
	std::vector <void*> vulkanUniformBuffersMapped{};

	FontPushConstants pushConstants{};

	VkPipelineLayout vulkanPipelineLayout = VK_NULL_HANDLE;
	VkPipeline vulkanGraphicsPipeline = VK_NULL_HANDLE;

	VkBuffer vertexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;

	VkBuffer indexBuffer = VK_NULL_HANDLE;
	VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;
	uint32_t indexCount = 0xFFFFFFFF;
	
	std::shared_ptr<Texture> texture = nullptr;
	std::shared_ptr<GlyphTexture> glyphTexture = nullptr;
};


struct ObjectRenderData
{

};