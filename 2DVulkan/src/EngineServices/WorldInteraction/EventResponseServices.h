#pragma once

#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"
#include "EngineData/RenderData/WindowStateData.h"
#include "EngineData/EventData/KeyInputData.h"

#include "EngineServices/WorldInteraction/BlockServices.h"

namespace Service
{
	class EventResponseServices
	{
	public:
		static void TestAllEventServices();

		static void PlaceBlockInWorld();
		static void MoveBlockInWorld();
		static void DeleteBlockInWorld();

		static void AddElementInSpellCreator();
		static void MoveElementInSpellCreator();
		static void DeleteElementInSpellCreator();

	};
}