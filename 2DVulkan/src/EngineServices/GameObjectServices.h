#pragma once
#include <glm.hpp>

#include "EngineData/ObjectData/GameObject.h"
#include "EngineData/ObjectData/WorldObjectsData.h"
#include "EngineData/ObjectData/Vertex.h"

#include "EngineServices/VulkanInitializerService.h"
#include "EngineServices/VulkanDestructorService.h"

#include "EngineData/EventData/KeyInputData.h"


namespace Service
{
	class GameObjectServices
	{
	public:
		static WorldObjectsData CreateGameObjects();
		static void CreateGameObject(glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 color);
	
		static int GetClickedOnGameObject();
	};
}