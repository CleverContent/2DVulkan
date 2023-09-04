#pragma once
#include <glm.hpp>

struct ObjectPlacementDock
{
	float Rotation = 0.0f;
	float Color[3] = {0.25, 0.75, 0.92};
	int selectedChar = 'A';
	int SelectedObjectID = -1;
	int ActivelyInteractingObjectID = -1;
	

	glm::vec2 MouseClickOffset = { 0, 0 };
};