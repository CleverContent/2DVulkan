#pragma once
#include <vulkan/vulkan.h>
#include <vector>

struct PushConstants
{
	PushConstants() {};
	void addPushConstant(VkShaderStageFlags shaderStage, int sizeInBytes)
	{
		shaderStages.push_back(shaderStage);
		structSizes.push_back(sizeInBytes);
	}

	
	std::vector<VkShaderStageFlags> shaderStages{};
	std::vector <int> structSizes{};
};