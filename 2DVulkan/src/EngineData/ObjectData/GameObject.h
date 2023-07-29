#pragma once
#include <glm.hpp>
#include <vulkan/vulkan.h>

#include "VulkanData.h"

struct GameObject
{
	int ObjectId = -1;

	glm::vec2 position = {0,0};
	glm::vec2 velociy = {0,0};
	glm::vec2 acceleration = {0,0};

	float rotation;//Always on z axis
	glm::vec2 scale = { 1,1 };
	glm::vec3 color = {0.5f, 0.5f, 0.5f};

	bool Static = true;

	VulkanData vulkanData{};


};