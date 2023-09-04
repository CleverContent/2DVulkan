#include "GameObjectServices.h"

WorldObjectsData Service::GameObjectServices::CreateGameObjects()
{
	return WorldObjectsData();
}


void Service::GameObjectServices::CreateGameObject(glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 color, int character)
{
	PublicVulkanData& publicVulkanData = *PublicVulkanData::getInstance();
	CoreVulkanData& coreVulkanData = *CoreVulkanData::getInstance();

	std::vector<GameObject>& gameObjects = publicVulkanData.getWorldObjectData().gameObjects;

	GameObject gameObject{};

	gameObject.ObjectId = publicVulkanData.getWorldObjectData().getNextID();

	gameObject.position = position;
	gameObject.velociy = { 0.0f, 100.0f };
	gameObject.acceleration = { 0.0f, 0.0f };
	gameObject.rotation = rotation;
	gameObject.scale = scale;
	gameObject.color = color;

	gameObject.vulkanData.texture = publicVulkanData.fontData.texture;
	gameObject.vulkanData.glyphTexture = publicVulkanData.fontData.glyphTexture;

	gameObject.vulkanData.pushConstants.glpyhId = character;

	Vertex v1 = { {0, 1}, color, {1.0f, 1.0f} };
	Vertex v2 = { {1, 1}, color, {0.0f, 1.0f} };
	Vertex v3 = { {0, 0}, color, {1.0f, 0.0f} };
	Vertex v4 = { {1, 0}, color, {0.0f, 0.0f} };

	std::vector<Vertex> vertices = { v1, v2, v3, v4 };
	std::vector<uint32_t> indices = { 0, 2, 1, 1, 2, 3 };

	Service::VulkanInitializerService::CreateObjectBuffers(gameObject.vulkanData, vertices, indices);

	Service::VulkanInitializerService::PopulateVulkanData(gameObject.vulkanData);

	gameObjects.push_back(gameObject);
}

int Service::GameObjectServices::GetClickedOnGameObject()
{
	PublicVulkanData* publicVulkanData = PublicVulkanData::getInstance();
	KeyInputData* eventData = publicVulkanData->eventData;

	std::vector<GameObject>& gameObjects = publicVulkanData->worldObjectsData->at(publicVulkanData->currentActiveWindow).gameObjects;

	for (int i = gameObjects.size() - 1; i >= 0; i--)
	{
		glm::vec2 mousePos = eventData->mousePosition;

		GameObject& gameObject = gameObjects.at(i);

		if (mousePos.x > fmin(gameObject.position.x, gameObject.position.x + (gameObject.scale.x)) &&
			mousePos.x < fmax(gameObject.position.x, gameObject.position.x + (gameObject.scale.x)) &&
			mousePos.y > fmin(gameObject.position.y, gameObject.position.y + (gameObject.scale.y)) &&
			mousePos.y < fmax(gameObject.position.y, gameObject.position.y + (gameObject.scale.y)))
		{
			return gameObject.ObjectId;
		}
	}
	return -1;
}
