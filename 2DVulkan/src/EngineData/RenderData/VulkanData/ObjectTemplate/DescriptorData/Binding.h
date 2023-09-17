#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "EngineData/RenderData/VulkanData/CoreVulkanData.h"

struct Binding
{

public:
	VkDescriptorSet getDescriptorSet()
	{
		CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();
		if (coreVulkanData == nullptr)
		{
			coreVulkanData = CoreVulkanData::getInstance();
		}
		int currentFrame = coreVulkanData->currentFrameAheadCount;

		return desctriptorSets[currentFrame];

	}


private:
	std::vector<VkDescriptorSet> desctriptorSets{};
};