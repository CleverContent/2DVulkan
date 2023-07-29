#include "ObjectController.h"

ObjectController::ObjectController()
{
}

ObjectController::ObjectController(ObjectControllerFlags flags, KeyInputData* eventData)
{
	this->eventData = eventData;

	for (int i = 0; i < Windows::UNKNOWN; i++)
	{
		worldObjectsData.insert({ (Windows)i, WorldObjectsData{} });
	}

}

ObjectController::~ObjectController()
{

}

void ObjectController::Init()
{
	vulkanData = CoreVulkanData::getInstance();
	publicVulkanData = PublicVulkanData::getInstance();
}

void ObjectController::ImGuiPlaceObject()
{
	
}

void ObjectController::Update()
{
	Service::ImGuiService::RunAllImGuiDocks();
	Service::EventResponseServices::TestAllEventServices();
}
