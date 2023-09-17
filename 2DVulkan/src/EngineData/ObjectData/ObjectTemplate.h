#pragma once
#include "DescriptorData/BindingLayout.h"
#include "PipelineData/Pipeline.h"
#include "PipelineData/PipelineLayout.h"
#include "ShaderStageDescriptorType.h"

struct ObjectTemplate
{
	ObjectTemplate(BindingLayout descriptorInfos, PipelineLayout pushConstantInfos)
	{

	}

	BindingLayout bindingLayout{};
	PipelineLayout pipelineLayout{};
	Pipeline pipeLine{};

};