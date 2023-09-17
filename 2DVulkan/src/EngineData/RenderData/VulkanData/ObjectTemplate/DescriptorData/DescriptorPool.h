#pragma once
#include <vulkan/vulkan.h>
#include <vector>

struct DescriptorPool
{
	int DescriptorCount = 0;
	VkDescriptorPool pool{};
};