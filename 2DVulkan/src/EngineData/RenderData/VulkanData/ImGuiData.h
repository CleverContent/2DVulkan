#pragma once

#include <vector>
#include <map>

#include "vulkan/vulkan.h"

struct ImGuiData
{
	VkRenderPass renderPass;

	VkDescriptorPool descriptorPool;

	VkCommandPool commandPool = VK_NULL_HANDLE;

	std::vector<VkCommandBuffer> commandBuffers{};

	std::vector<VkFramebuffer> frameBuffers{};

	std::map<void*, void*> ImGUiFrameFuncitons{};
};