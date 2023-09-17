#pragma once
#include <map>
#include <vector>
#include <vulkan/vulkan.h>
#include <glm.hpp>

#include  "EngineData/RenderData/VulkanData/ObjectTemplate/VertexDataInfo.h"

const static std::map<VertexDataTypes, int> VertexDataSizes =
{
	{VertexDataTypes::FLOAT, sizeof(float)},
	{VertexDataTypes::INT, sizeof(int)},
	{VertexDataTypes::VEC2, sizeof(glm::vec2)},
	{VertexDataTypes::VEC3, sizeof(glm::vec3)},
	{VertexDataTypes::VEC4, sizeof(glm::vec4)}
};

const static std::map<VertexDataTypes, VkFormat> VertexDataFormat =
{
	{VertexDataTypes::FLOAT, VK_FORMAT_R32_SFLOAT},
	{VertexDataTypes::INT, VK_FORMAT_R32_SINT},
	{VertexDataTypes::VEC2, VK_FORMAT_R32G32_SFLOAT},
	{VertexDataTypes::VEC3, VK_FORMAT_R32G32B32_SFLOAT},
	{VertexDataTypes::VEC4, VK_FORMAT_R32G32B32A32_SFLOAT}
};

struct Vertex
{
	
	void addVertexAttribute(VertexDataInfo dataInfo)
	{
		info.push_back(dataInfo);
	}

	VkVertexInputBindingDescription getBindingDescription() {
		int size = 0;

		for (int i = 0; i < info.size(); i++)
		{
			size += VertexDataSizes.at(info.at(i).type);
		}

		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = size;
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	std::vector<VkVertexInputAttributeDescription> getVertexAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(info.size());

		int currentOffset = 0;

		for (int i = 0; i < info.size(); i++)
		{
			VertexDataInfo data = info.at(i);
			attributeDescriptions.at(i).binding = data.bindingIndex;
			attributeDescriptions.at(i).location = data.locationIndex;
			attributeDescriptions.at(i).format = VertexDataFormat.at(data.type);
			attributeDescriptions.at(i).offset = currentOffset;

			currentOffset += VertexDataSizes.at(data.type);
		}

		return attributeDescriptions;
	}

	std::vector<VertexDataInfo> info{};
};