#include "BlockServices.h"

int Service::BlockServices::CreateBlock(GameObject gameObject)
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();
	CoreVulkanData* coreVulkanData = CoreVulkanData::getInstance();


	int objectId = publicVulkanData->getWorldObjectData().getLastUsedID() + 1;
	for (GameObject gameObject : publicVulkanData->getWorldObjectData().gameObjects)
	{
		if (gameObject.ObjectId == objectId)
		{
			throw std::runtime_error("Trying to add a Block with the same Id as an existing Block");
		}
	}

	GameObjectServices::CreateGameObject(gameObject.position, gameObject.rotation, gameObject.scale, gameObject.color);
	return objectId;
}

void Service::BlockServices::RemoveBlock(int index)
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();

	int i = 0;
	for (GameObject& gameObject : publicVulkanData->getWorldObjectData().gameObjects)
	{
		if (gameObject.ObjectId == index)
		{
			publicVulkanData->getWorldObjectData().gameObjects.erase(publicVulkanData->getWorldObjectData().gameObjects.begin() + i);
			return;
		}

		i++;
	}
	
	throw std::runtime_error("Trying to Remove Block that Does not exist");
}

int Service::BlockServices::SelectBlock()
{
	return GameObjectServices::GetClickedOnGameObject();
}

void Service::BlockServices::MoveBlock(int index, glm::vec2 newPos)
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();

	for (GameObject& gameObject : publicVulkanData->getWorldObjectData().gameObjects)
	{
		if (gameObject.ObjectId == index)
		{
			gameObject.position = newPos;
			return;
		}
	}

	throw std::runtime_error("You just Tried to move a Block that does not exist");
}

void Service::BlockServices::ResizeBlock(int index, glm::vec2 newScale)
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();

	for (GameObject& gameObject : publicVulkanData->getWorldObjectData().gameObjects)
	{
		if (gameObject.ObjectId == index)
		{
			gameObject.scale = newScale;
			return;
		}
	}
}

