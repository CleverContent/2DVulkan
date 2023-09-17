#pragma once

#include "EngineData/ObjectData/GameObjects/GameObject.h"


namespace Service
{
	class ObjectManager
	{
	public:
		static GameObject CreateBlankObject();
		static void UpdateObjectRenderData();

	};
}