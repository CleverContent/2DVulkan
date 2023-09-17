#pragma once

#include "EngineData/ObjectData/GameObjects/GameObject.h"
#include "EngineData/RenderData/VulkanData/CoreVulkanData.h"
#include "EngineData/RenderData/VulkanData/ObjectTemplate/DescriptorData/DescriptorPool.h"
#include "EngineData/RenderData/VulkanData/ObjectTemplate/DescriptorData/BindingLayout.h"
#include "EngineData/RenderData/VulkanData/ObjectTemplate/PipelineData/PipelineLayout.h"
#include "EngineData/RenderData/VulkanData/ObjectTemplate/PipelineData/Pipeline.h"

namespace Service
{
	class GameObjectService
	{
	public:
		static GameObject CreateBlankObject();
		static void UpdateObjectRenderData();

		static void CreateDesciptorPool(DescriptorPool& descriptorPool);

		static void CreateBindingLayout(BindingLayout& bindingLayout);
		static void CreatePipelineLayout(BindingLayout& bindingLayout, PipelineLayout& pipelineLayout);
		static void CreatePipeline(BindingLayout& bindingLayout, PipelineLayout& pipelineLayout);

	};
}