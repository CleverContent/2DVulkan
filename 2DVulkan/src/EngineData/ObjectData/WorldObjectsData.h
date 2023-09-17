#pragma once
#include <vector>
#include <map>

#include "GameObject.h"
#include "EngineData/RenderData/WindowStateData.h"

struct WorldObjectsData
{
	std::vector<GameObject> gameObjects{};

	GameObject& getGameObject(int id)
	{
		for (GameObject& gameObject : gameObjects)
		{
			if (gameObject.ObjectId == id)
				return gameObject;
		}
	}

	int getNextID()
	{
		return ++NextObjectID;
	}

	int getLastUsedID()
	{
		return NextObjectID;
	}

private:
	int NextObjectID = -1;
};