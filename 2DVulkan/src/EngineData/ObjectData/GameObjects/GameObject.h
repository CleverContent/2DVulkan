#pragma once
#include <glm.hpp>

#include "GameObjectRenderData.h"
#include "GameObjectPhysicsData.h"
#include "GameObjectShaderData.h"

struct GameObject
{
	glm::uint32 objectID = 0;

	GameObjectRenderData renderData{};
	GameObjectPhysicsData physicsData{};
	GameObjectShaderData shaderData{};
};
