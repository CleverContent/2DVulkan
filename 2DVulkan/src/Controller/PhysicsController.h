#pragma once
#include "PhysicsControllerFlags.h"
#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"

class PhysicsController
{

public:
	PhysicsController() {};
	PhysicsController(PhysicsControllerFlags PCF);
	~PhysicsController();

	void Init();

	void Update();

	bool CheckCollisions(GameObject& gameObject);
	bool CheckPointInSquare(glm::vec2 point, glm::vec2 topRightCornor, glm::vec2 bottomLeftCornor);
};