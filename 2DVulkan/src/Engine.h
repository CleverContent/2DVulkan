#pragma once
#include <chrono>


#include "Controller/RenderControllerFlags.h"
#include "Controller/RenderController.h"

#include "Controller/ObjectController.h"

#include "Controller/EventController.h"
#include "Controller/EventControllerFlags.h"

#include "EngineData/EngineStartFlags.h"
#include "EngineData/EngineData.h"

#include "Controller/PhysicsController.h"
#include "Controller/PhysicsControllerFlags.h"

#include "Controller/MagicController.h"
#include "Controller/MagicControllerFlags.h"


class Engine
{
public:
	Engine(EngineStartFlags flags);
	~Engine();

	void Update();

	void WindowSwitcher();

	EngineData GetEngineData();
	bool DidEngineEnd();

private:
	ObjectController objectController;
	RenderController renderController;
	EventController eventController;
	PhysicsController physicsController;
	MagicController magicController;
	
	std::chrono::steady_clock::time_point startTime;


};