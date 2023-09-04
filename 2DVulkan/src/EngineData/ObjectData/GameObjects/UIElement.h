#pragma once
#include <vector>
#include <string>
#include <glm.hpp>

#include "GameObject.h"


struct UIElement : GameObject
{
	std::map<std::string, glm::vec2> textPositions{};


	VulkanData& getRenderData()
	{

	}
};