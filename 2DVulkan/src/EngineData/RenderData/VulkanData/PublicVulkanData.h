#pragma once
#include <glm.hpp>
#include <map>
#include <string>

#include "EngineData/ObjectData/WorldObjectsData.h"
#include "EngineData/RenderData/WindowStateData.h"
#include "ImGuiDockData/DockData.h"
#include "CoreVulkanData.h"
#include "EngineData/EventData/KeyInputData.h"
#include "EngineData/RenderData/FontData.h"

struct PublicVulkanData
{
	PublicVulkanData(const PublicVulkanData& obj) = delete;

	static PublicVulkanData* getInstance()
	{
		if (instance == nullptr)
		{
			instance = new PublicVulkanData{};
		}
		return instance;
	}

	std::map<Windows, WorldObjectsData>* worldObjectsData;
	glm::vec2 windowSize = { -1, -1 };
	float LastFrameTime = 0.0f;

	Windows currentActiveWindow = WORLD;

	DockData ImGuiDockData{};

	KeyInputData* eventData;

	WorldObjectsData& getWorldObjectData()
	{
		return worldObjectsData->at(currentActiveWindow);
	}

private:
	static PublicVulkanData* instance;
};
