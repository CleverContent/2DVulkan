#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <map>
#include "EngineData/RenderData/VulkanData/ObjectTemplate/ShaderStageDescriptorType.h"


struct BindingLayout
{

	BindingLayout() {};
	BindingLayout(std::vector<ShaderStageDescriptorType> types)
	{
		for (ShaderStageDescriptorType t : types)
		{
			bindingLayout.insert({ t.bindingIndex, t});
		}
	}

	void addDescriptorType(ShaderStageDescriptorType type)
	{
		bindingLayout.insert({ type.bindingIndex, type });
	}

	int getDescriptorCount()
	{
		return bindingLayout.size();
	}

	ShaderStageDescriptorType getDescriptor(int index)
	{
		return bindingLayout.at(index);
	}
	
	VkDescriptorSetLayout& getDescriptorSetLayout()
	{
		return layout;
	}

protected:
	std::map<int, ShaderStageDescriptorType> bindingLayout{};

	VkDescriptorSetLayout layout{};
	VkPipeline pipeline{};
};