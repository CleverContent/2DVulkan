#pragma once
#include <glm.hpp>
#include "EngineData/ObjectData/VulkanData.h"

struct GameObject
{
	glm::vec2 position{};
	glm::vec2 rotation{};
	glm::vec2 scale{};

	GameObject(glm::vec2 p, glm::vec2 r, glm::vec2 s)
	{
		position = p;
		rotation = r;
		scale = s;
	}

	virtual VulkanData& getRenderData() = 0;
};