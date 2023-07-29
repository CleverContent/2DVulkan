#pragma once
#include <imgui.h>

#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"
#include "EngineServices/MagicInteraction/ImGuiDocks/SpellBlockAdder.h"

namespace Service
{

	class ImGuiService
	{
	public:
		static void RunAllImGuiDocks();


		static void ObjectInteraction();
		static void SpellBlockInteraction();
	};
}
