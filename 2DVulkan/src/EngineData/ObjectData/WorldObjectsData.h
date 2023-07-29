#pragma once
#include <vector>
#include <map>

#include "GameObject.h"
#include "EngineData/RenderData/WindowStateData.h"

enum ObjectState
{
	STATIC,
	NO_GRAVITY,
	RIGID_BODY
};

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