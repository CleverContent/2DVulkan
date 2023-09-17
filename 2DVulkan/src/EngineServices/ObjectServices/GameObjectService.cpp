#include "GameObjectService.h"

void Service::GameObjectService::CreateDesciptorPool(DescriptorPool& descriptorPool)
{
	CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();
	if (coreVulkanData == nullptr)
	{
		coreVulkanData = CoreVulkanData::getInstance();
	}

    std::vector<VkDescriptorPoolSize> sizeInfo(2);
	sizeInfo[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	sizeInfo[0].descriptorCount = 1000;
	sizeInfo[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	sizeInfo[1].descriptorCount = 1000;

	VkDescriptorPoolCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	info.poolSizeCount = static_cast<uint32_t>(sizeInfo.size());
	info.pPoolSizes = sizeInfo.data();
	info.maxSets = 1000;

	if (vkCreateDescriptorPool(coreVulkanData->device, &info, nullptr, &descriptorPool.pool) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor pool");
	}
}

void Service::GameObjectService::CreateBindingLayout(BindingLayout& bindingLayout)
{
	CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();
	if (coreVulkanData == nullptr)
	{
		coreVulkanData = CoreVulkanData::getInstance();
	}

	int descriptorCount = bindingLayout.getDescriptorCount();

	std::vector<VkDescriptorSetLayoutBinding> bindingInfos(descriptorCount);

	for (int i = 0; i < descriptorCount; i++)
	{
		ShaderStageDescriptorType descriptorInfo = bindingLayout.getDescriptor(i);

		bindingInfos.at(i).binding = descriptorInfo.bindingIndex;
		bindingInfos.at(i).descriptorType = descriptorInfo.descriptorType;
		bindingInfos.at(i).descriptorCount = 1;
		bindingInfos.at(i).stageFlags = descriptorInfo.shaderStage;
	}

	VkDescriptorSetLayoutCreateInfo desciptorInfo{};
	desciptorInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	desciptorInfo.bindingCount = static_cast<uint32_t>(bindingInfos.size());
	desciptorInfo.pBindings = bindingInfos.data();


	if (vkCreateDescriptorSetLayout(coreVulkanData->device, &desciptorInfo, nullptr, &bindingLayout.getDescriptorSetLayout()) != VK_SUCCESS)
	{
		throw std::runtime_error("DesctiptorSetLayout failed to be created!");
	}
}

void Service::GameObjectService::CreatePipelineLayout(BindingLayout& bindingLayout, PipelineLayout& pipelineLayout)
{
	CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();
	if (coreVulkanData == nullptr)
	{
		coreVulkanData = CoreVulkanData::getInstance();
	}

	PushConstants& pushConstants = pipelineLayout.pushConstants;

	std::vector<VkPushConstantRange> pushConstantRanges(pushConstants.shaderStages.size());
	for (int i = 0; i < pushConstants.shaderStages.size(); i++)
	{
		pushConstantRanges.at(i).offset = 0;
		pushConstantRanges.at(i).size = pushConstants.structSizes.at(i);
		pushConstantRanges.at(i).stageFlags = pushConstants.shaderStages.at(i);
	}


	VkPipelineLayoutCreateInfo info{};
	info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	info.setLayoutCount = 1;
	info.pSetLayouts = &bindingLayout.getDescriptorSetLayout();
	info.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
	info.pPushConstantRanges = pushConstantRanges.data();

	if (vkCreatePipelineLayout(coreVulkanData->device, &info, nullptr, &pipelineLayout.pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Pipeline Layout failed to be created!");
	}
}

void Service::GameObjectService::CreatePipeline(BindingLayout& bindingLayout, PipelineLayout& pipelineLayout)
{
	CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();
	if (coreVulkanData == nullptr)
	{
		coreVulkanData = CoreVulkanData::getInstance();
	}

	auto vertShaderCode = ReadShaderFile("src/Resources/Shaders/" + shaderFileName + ".spv");
	auto fragShaderCode = ReadShaderFile("src/Resources/Shaders/" + shaderFileName + ".spv");

	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStageCreateInfo[] = { vertShaderStageInfo, fragShaderStageInfo };

	auto bindingDescription = Vertex::getBindingDescription();
	auto attributeDescriptions = Vertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());

	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;

	//rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineDepthStencilStateCreateInfo depthStencil{};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	VkGraphicsPipelineCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	createInfo.stageCount = 2;
	createInfo.pStages = shaderStageCreateInfo;
	createInfo.pVertexInputState = &vertexInputInfo;
	createInfo.pInputAssemblyState = &inputAssembly;
	createInfo.pViewportState = &viewportState;
	createInfo.pRasterizationState = &rasterizer;
	createInfo.pMultisampleState = &multisampling;
	createInfo.pColorBlendState = &colorBlending;
	createInfo.pDynamicState = &dynamicState;
	createInfo.pDepthStencilState = &depthStencil;
	createInfo.layout = vulkanData.vulkanPipelineLayout;
	createInfo.renderPass = coreVulkanData->renderPass;
	createInfo.subpass = 0;
	createInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(coreVulkanData->device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &vulkanData.vulkanGraphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Graphics Pipeline");
	}

	vkDestroyShaderModule(coreVulkanData->device, fragShaderModule, nullptr);
	vkDestroyShaderModule(coreVulkanData->device, vertShaderModule, nullptr);
}
