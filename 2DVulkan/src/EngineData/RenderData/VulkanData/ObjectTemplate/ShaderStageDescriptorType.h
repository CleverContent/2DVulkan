#pragma once
#include <vulkan/vulkan.h>

struct ShaderStageDescriptorType
{
	VkShaderStageFlags shaderStage{};
	VkDescriptorType descriptorType{};
	int bindingIndex = 0;
	int setIndex = 0;
};