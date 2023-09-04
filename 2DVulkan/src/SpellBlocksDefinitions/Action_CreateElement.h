#pragma once
#include <iostream>
#include "EngineServices/WorldInteraction/BlockServices.h"
#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"
#include "BlockDataParent.h"

#include "EngineData/EventData/Interface_OnClick.h"


struct Event_CreateElement : SpellBlock
{
	glm::vec2* scale = nullptr;
	glm::vec3* color = nullptr;
	glm::vec2* position = nullptr;
	int* objectId = nullptr;

	std::string name = "CREATE_ELEMENT ACTION";

	void run()
	{
		GameObject gameObject{};

		gameObject.position = *position;
		gameObject.scale = *scale;
		gameObject.color = *color;

		Service::BlockServices::CreateBlock(gameObject);
		
	};

	void start(SpellActionData parentData)
	{
		data = parentData;
		scale = (glm::vec2*)addnewTypeToData(VECTOR2);
		color = (glm::vec3*)addnewTypeToData(VECTOR3);
		position = &data.v_Vec2.at(0);
		objectId = (int*)addnewTypeToData(INT);
		run();
	};

	virtual std::string getString()
	{
		return name;
	}
};