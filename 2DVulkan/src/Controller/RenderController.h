#pragma once
#include <map>

#include "RenderControllerFlags.h"

#include "EngineData/RenderData/VulkanData/CoreVulkanData.h"
#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"

#include "EngineServices/WindowServices.h"
#include "EngineServices/VulkanInitializerService.h"
#include "EngineData/ObjectData/WorldObjectsData.h"

#include "EngineData/EventData/KeyInputData.h"

class RenderController
{
public:



	RenderController() {};
	RenderController(RenderControllerFlags rcf, std::map<Windows, WorldObjectsData>* objectsData, KeyInputData* eventData);
	~RenderController();

	void StartUpdate();
	void FinishUpdate();

	void Render();

	bool HasWindowClosed();

private:
	std::map<Windows, WorldObjectsData>* worldObjectsData;

public:
	KeyInputData* eventData = nullptr;

	CoreVulkanData* coreVulkanData = nullptr;
	PublicVulkanData* publicVulkanData = nullptr;
};