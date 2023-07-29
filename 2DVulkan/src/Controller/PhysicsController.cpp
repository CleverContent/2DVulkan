#include "PhysicsController.h"

PhysicsController::PhysicsController(PhysicsControllerFlags PCF)
{
}

PhysicsController::~PhysicsController()
{

}

void PhysicsController::Init()
{
}

void PhysicsController::Update()
{
	PublicVulkanData* publicData = PublicVulkanData::getInstance();

	std::vector<GameObject>& gameObjects = publicData->worldObjectsData->at(publicData->currentActiveWindow).gameObjects;

	for (auto& gameObject : gameObjects)
	{
		if (!gameObject.Static)
		{

			bool IsColliding = CheckCollisions(gameObject);
			if (!IsColliding)
			{
				gameObject.position += gameObject.velociy * publicData->LastFrameTime;
				gameObject.velociy += gameObject.acceleration * publicData->LastFrameTime;
			}
			/*else
			{
				publicVulkanData->worldObjectsData->gameObjectStates[key] = false;
			}*/
		}
	}
}

bool PhysicsController::CheckCollisions(GameObject& go)
{
	PublicVulkanData* publicData = PublicVulkanData::getInstance();

	std::vector<GameObject>& gameObjects = publicData->worldObjectsData->at(publicData->currentActiveWindow).gameObjects;

	for (GameObject gameObject : gameObjects)
	{
		if (gameObject.ObjectId == go.ObjectId)
			continue;

		if (CheckPointInSquare(go.position, gameObject.position, gameObject.position + gameObject.scale * 10.0f))
			return true;
		else if (CheckPointInSquare(go.position + glm::vec2{go.scale.x * 10.0f, 0.0f}, gameObject.position, gameObject.position + gameObject.scale * 10.0f))
			return true;
		else if (CheckPointInSquare(go.position + glm::vec2{ 0.0f, go.scale.y * 10.0f }, gameObject.position, gameObject.position + gameObject.scale * 10.0f))
			return true;
		else if (CheckPointInSquare(go.position + glm::vec2{ go.scale.x * 10.0f, go.scale.y * 10.0f }, gameObject.position, gameObject.position + gameObject.scale * 10.0f))
			return true;

		/*if (CheckPointInSquare(gameObject.position, gameObject.position, go.position + go.scale * 10.0f))
			return true;
		else if (CheckPointInSquare(gameObject.position + glm::vec2{ gameObject.scale.x * 10.0f, 0.0f }, go.position, go.position + go.scale * 10.0f))
			return true;
		else if (CheckPointInSquare(gameObject.position + glm::vec2{ 0.0f, gameObject.scale.y * 10.0f }, go.position, go.position + go.scale * 10.0f))
			return true;
		else if (CheckPointInSquare(gameObject.position + glm::vec2{ gameObject.scale.x * 10.0f, go.scale.y * 10.0f }, go.position, go.position + go.scale * 10.0f))
			return true;*/
	}
	return false;
}

bool PhysicsController::CheckPointInSquare(glm::vec2 point, glm::vec2 topRightCornor, glm::vec2 bottomLeftCornor)
{
	if (point.x > fmin(topRightCornor.x, bottomLeftCornor.x) && point.x < fmax(topRightCornor.x, bottomLeftCornor.x) &&
		point.y > fmin(topRightCornor.y, bottomLeftCornor.y) && point.y < fmax(topRightCornor.y, bottomLeftCornor.y))
	{
		return true;
	}
	return false;
}
