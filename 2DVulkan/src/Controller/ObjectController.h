#pragma once

#include <math.h>

#include "imgui.h"
#include "gtx/string_cast.hpp"

#include "EngineServices/ImGui/ImGuiService.h"

#include "ImGuiDockData/DockData.h"

#include "ObjectControllerFlags.h"
#include "EngineData/ObjectData/WorldObjectsData.h"

#include "EngineServices/GameObjectServices.h"
#include "EngineData/RenderData/VulkanData/CoreVulkanData.h"

#include "EngineData/EventData/KeyInputData.h"
#include "Controller/EventController.h"

#include "EngineData/RenderData/WindowStateData.h"

#include "EngineServices/WorldInteraction/EventResponseServices.h"

#include "EngineServices/TextureService/TextureInit.h"

#include "EngineData/RenderData/FontData.h"

class ObjectController
{
public:
	ObjectController();
	ObjectController(ObjectControllerFlags flags, KeyInputData* eventData);
	~ObjectController();

	void Init();
	void Update();

	inline std::map<Windows, WorldObjectsData>* getAllWorldsObjectsData() { return &worldObjectsData; };

	inline WorldObjectsData* getWorldObjectsData(Windows window) { return &worldObjectsData.at(window); };

private:
	void ImGuiPlaceObject();

private:
	DockData dockData{};

	KeyInputData* eventData = nullptr;

	CoreVulkanData* vulkanData = nullptr;
	PublicVulkanData* publicVulkanData = nullptr;

	std::map<Windows, WorldObjectsData> worldObjectsData{};
};