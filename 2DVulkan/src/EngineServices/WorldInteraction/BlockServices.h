#pragma once
#include <vector>
#include <glm.hpp>
#include <stdexcept>

#include "EngineServices/GameObjectServices.h"

#include "EngineData/EventData/KeyInputData.h"
#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"

namespace Service
{
	class BlockServices
	{
	public:

		//These Services are only for after Event Input checking IE mouse click or keyboard press or something

		static int CreateBlock(GameObject gameObject);
		static void RemoveBlock(int index);
		static int SelectBlock();
		static void MoveBlock(int index, glm::vec2 newPos);
		static void ResizeBlock(int index, glm::vec2 newSize);
	};
}