#include "ImGuiService.h"

namespace Service
{

	void ImGuiService::RunAllImGuiDocks()
	{
		PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();

		if (publicVulkanData->currentActiveWindow == WORLD)
		{
			ObjectInteraction();
		}
		else if(publicVulkanData->currentActiveWindow == SPELLCREATOR)
		{
			SpellBlockAdderData sbad{};
			SpellBlockAdder::RunSpellBlockAdderDock(&sbad);
		}
	}


	void ImGuiService::ObjectInteraction()
	{
		PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();

		ImGui::Begin("Create Object");

		ImGui::DragFloat("New Object Rotation: ", &publicVulkanData->ImGuiDockData.objectPlacementDock.Rotation, 1.0f, 0.0f, 360.0f);
		ImGui::ColorPicker3("Object Color: ", publicVulkanData->ImGuiDockData.objectPlacementDock.Color);

		ImGui::End();
	}

	void ImGuiService::SpellBlockInteraction()
	{



	}
}