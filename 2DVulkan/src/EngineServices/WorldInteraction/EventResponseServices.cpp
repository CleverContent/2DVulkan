#include "EventResponseServices.h"

void Service::EventResponseServices::TestAllEventServices()
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();
	
	Windows window = publicVulkanData->currentActiveWindow;

	switch (window)
	{
	case(WORLD):
		PlaceBlockInWorld();
		MoveBlockInWorld();
		DeleteBlockInWorld();
		break;

	case(SPELLCREATOR):
		AddElementInSpellCreator();
		MoveElementInSpellCreator();
		DeleteElementInSpellCreator();
		break;

	default:
		break;
	}
}

void Service::EventResponseServices::PlaceBlockInWorld()
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();
	KeyInputData& eventData = *publicVulkanData->eventData;

	if (eventData.keys[KEY_LEFT_CONTROL] == PRESSED)
	{
		if (eventData.mouseButtons[BUTTON_1] == CLICKED)
		{
			GameObject gameObject{};

			gameObject.position = eventData.mousePosition;
			gameObject.rotation = publicVulkanData->ImGuiDockData.objectPlacementDock.Rotation;
			gameObject.color = { publicVulkanData->ImGuiDockData.objectPlacementDock.Color[0], publicVulkanData->ImGuiDockData.objectPlacementDock.Color[1], publicVulkanData->ImGuiDockData.objectPlacementDock.Color[2] };
			gameObject.vulkanData.pushConstants.glpyhId = publicVulkanData->fontData.charsToGlyphIndex.at(publicVulkanData->ImGuiDockData.objectPlacementDock.selectedChar);

			int id = BlockServices::CreateBlock(gameObject);
			publicVulkanData->ImGuiDockData.objectPlacementDock.ActivelyInteractingObjectID = id;
			publicVulkanData->ImGuiDockData.objectPlacementDock.SelectedObjectID = id;
		}
		
		else if (eventData.mouseButtons[BUTTON_1] == PRESSED)
		{
			int objectId = publicVulkanData->ImGuiDockData.objectPlacementDock.ActivelyInteractingObjectID;

			glm::vec2 objectPos = publicVulkanData->getWorldObjectData().getGameObject(objectId).position;
			glm::vec2 mousePos = eventData.mousePosition;

			BlockServices::ResizeBlock(objectId, (mousePos - objectPos));
		}

		else if (eventData.mouseButtons[BUTTON_1] == RELEASED)
		{
			publicVulkanData->ImGuiDockData.objectPlacementDock.ActivelyInteractingObjectID = -1;
		}
	}
}

void Service::EventResponseServices::MoveBlockInWorld()
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();
	KeyInputData& eventData = *publicVulkanData->eventData;

	if (eventData.keys[KEY_LEFT_SHIFT] == PRESSED)
	{
		if (eventData.mouseButtons[BUTTON_1] == CLICKED)
		{
			int index = BlockServices::SelectBlock();
			if (index != -1)
			{
				publicVulkanData->ImGuiDockData.objectPlacementDock.ActivelyInteractingObjectID = index;

				glm::vec2 objectPos = publicVulkanData->getWorldObjectData().getGameObject(index).position;
				glm::vec2 mousePos = eventData.mousePosition;

				publicVulkanData->ImGuiDockData.objectPlacementDock.MouseClickOffset = mousePos - objectPos;

			}
		}

		if (eventData.mouseButtons[BUTTON_1] == PRESSED)
		{

			ObjectPlacementDock opd = publicVulkanData->ImGuiDockData.objectPlacementDock;

			if (opd.ActivelyInteractingObjectID != -1)
			{
				BlockServices::MoveBlock(opd.ActivelyInteractingObjectID, eventData.mousePosition - opd.MouseClickOffset);
			}
		}

		if (eventData.mouseButtons[BUTTON_1] == RELEASED)
		{
			publicVulkanData->ImGuiDockData.objectPlacementDock.ActivelyInteractingObjectID = -1;
		}
	}
}

void Service::EventResponseServices::DeleteBlockInWorld()
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();
	KeyInputData& eventData = *publicVulkanData->eventData;

	if (eventData.keys[KEY_LEFT_CONTROL] == PRESSED)
	{
		if (eventData.mouseButtons[BUTTON_2] == CLICKED)
		{
			int index = BlockServices::SelectBlock();
			if (index != -1)
			{
				BlockServices::RemoveBlock(index);

				if (index == publicVulkanData->ImGuiDockData.objectPlacementDock.ActivelyInteractingObjectID)
					publicVulkanData->ImGuiDockData.objectPlacementDock.ActivelyInteractingObjectID = -1;
			}

		}
	}
}

void Service::EventResponseServices::AddElementInSpellCreator()
{
}

void Service::EventResponseServices::MoveElementInSpellCreator()
{
}

void Service::EventResponseServices::DeleteElementInSpellCreator()
{
}
