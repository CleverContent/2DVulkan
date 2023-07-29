#pragma once
#include <iostream>

#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"
#include "BlockDataParent.h"

#include "EngineData/EventData/Interface_OnClick.h"


struct Event_OnClick : Interface_OnClick, SpellBlock
{
	bool* delay = nullptr;
	glm::vec2* clickPos = nullptr;

	std::string name = "ON_CLICK EVENT";

	void run()
	{
		
	};

	void start()
	{
		PublicVulkanData* publicData = PublicVulkanData::getInstance();

		//REGISTER ONCLICK EVENT
		publicData->eventData->callbacks.insertClickCallback(0, this);
		data.publicData = publicData;
		delay = (bool*)addnewTypeToData(BOOL);
		clickPos = (glm::vec2*)addnewTypeToData(VECTOR2);

	};

	virtual std::string getString()
	{
		return name;
	}

	virtual void OnClickCallBack(glm::vec2 mousePos)
	{
		data.publicData->eventData->callbacks.removeClickCallback(0, this);

		*clickPos = mousePos;
		run();
	}
};