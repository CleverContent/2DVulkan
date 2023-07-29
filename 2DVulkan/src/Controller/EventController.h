#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <functional>
#include<gtx/string_cast.hpp>

#include "EngineData/EventData/EventData.h"
#include "EngineData/EventData/KeyInputData.h"
#include "EventControllerFlags.h"
#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"

class EventController
{
public:
	EventController() {};
	EventController(EventControllerFlags ecf);

	void Init(GLFWwindow* glfwWindow);
	void Update();
	void CleanUp();

	static bool isModified(KeyInputData& keyData);

	void setKey(InputCodes::Keyboard key, Keystate state);
	void setMouseButton(InputCodes::Mouse button, Keystate state);
	void setMousePosition(glm::vec2 pos);

	KeyInputData* EventData()
	{
		return &keyInputData;
	}

public:
	GLFWwindow* glfwWindow = nullptr;
	KeyInputData keyInputData{};
private:

};