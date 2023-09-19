#pragma once
#include <vector>
#include <map>

#include "EngineData/ObjectData/GameObjects/GameObject.h"
#include "EngineData/RenderData/WindowStateData.h"

struct WorldObjectsData
{
	int getNextFreeObjectID()
	{
		return nextFreeObjectId++;
	}

	bool addGameObject(GameObject newGameObject)
	{
		if (gameObjects.count(newGameObject.objectID) != 0)
		{
			std::cout << "FAILED: could not add object because current object exists witht the same object ID \n";
			return false;
		}
		gameObjects.insert({ newGameObject.objectID, newGameObject});
	}

private:
	std::map<int, GameObject> gameObjects{};
	int nextFreeObjectId = 0;
};