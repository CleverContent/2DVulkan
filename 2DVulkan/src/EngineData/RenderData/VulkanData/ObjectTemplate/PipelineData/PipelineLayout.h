#pragma once
#include <vulkan/vulkan.h>
#include "EngineData/RenderData/VulkanData/ObjectTemplate/PushConstants.h"

struct PipelineLayout
{
	PipelineLayout() {};

	PipelineLayout(PushConstants pushConstants)
		: pushConstants(pushConstants)
	{

	}

	PushConstants pushConstants{};
	VkPipelineLayout pipelineLayout{};
};