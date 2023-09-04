#include "EventController.h"

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	EventController* eventController = static_cast<EventController*>(glfwGetWindowUserPointer(window));
	try {
		InputCodes::Keyboard cleverKey = (InputCodes::Keyboard)keyboardGLFWtoCleverKeyCodes.at(key);

		if (action == GLFW_PRESS)
			eventController->setKey(cleverKey, CLICKED);
		else if (action == GLFW_RELEASE)
			eventController->setKey(cleverKey, RELEASED);
	}
	catch (...)
	{
		std::cout << "YOU PRESSED A NO NO BUTTON, A key I didnt recognize, put I no crash :)" << std::endl;
	}
}

static void mouseButton_callback(GLFWwindow* window, int key, int action, int mods)
{
	EventController* eventController = static_cast<EventController*>(glfwGetWindowUserPointer(window));

	InputCodes::Mouse cleverMouseButton = (InputCodes::Mouse)mouseGLFWtoCleverButtonCodes.at(key);

	if (action == GLFW_PRESS)
	{
		eventController->setMouseButton(cleverMouseButton, CLICKED);
		eventController->keyInputData.callbacks.checkClickCallbacks(cleverMouseButton, eventController->keyInputData.mousePosition);
	}
	else if(action == GLFW_RELEASE)
		eventController->setMouseButton(cleverMouseButton, RELEASED);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	EventController* eventController = static_cast<EventController*>(glfwGetWindowUserPointer(window));

	eventController->setMousePosition({(float)xpos, (float)ypos});
}

EventController::EventController(EventControllerFlags ecf)
{

}

void EventController::Init(GLFWwindow* glfwWindow)
{
	this->glfwWindow = glfwWindow;

	PublicVulkanData::getInstance()->eventData = &keyInputData;

	glfwSetWindowUserPointer(glfwWindow, this);
	glfwSetKeyCallback(glfwWindow, key_callback);
	glfwSetMouseButtonCallback(glfwWindow, mouseButton_callback);
	glfwSetCursorPosCallback(glfwWindow, cursor_position_callback);
	
}

void EventController::Update()
{
	for (const auto& [key, value] : keyboardGLFWtoCleverKeyCodes)
	{
		if (keyInputData.keys[value] == CLICKED)
		{
			keyInputData.keys[value] = PRESSED;
		}
		if (keyInputData.keys[value] == RELEASED)
		{
			keyInputData.keys[value] = NOT_PRESSED;
		}
	}

	for (const auto& [key, value] : mouseGLFWtoCleverButtonCodes)
	{
		if (keyInputData.mouseButtons[value] == CLICKED)
		{
			keyInputData.mouseButtons[value] = PRESSED;
		}

		if (keyInputData.mouseButtons[value] == RELEASED)
		{
			keyInputData.mouseButtons[value] = NOT_PRESSED;
		}
	}

	glfwPollEvents();
}

void EventController::CleanUp()
{

}

bool EventController::isModified(KeyInputData& keyData)
{
	if (keyData.keys[KEY_LEFT_ALT] != NOT_PRESSED)
		return true;
	if (keyData.keys[KEY_RIGHT_ALT] != NOT_PRESSED)
		return true;
	if (keyData.keys[KEY_LEFT_CONTROL] != NOT_PRESSED)
		return true;
	if (keyData.keys[KEY_RIGHT_CONTROL] != NOT_PRESSED)
		return true;
	if (keyData.keys[KEY_LEFT_SHIFT] != NOT_PRESSED)
		return true;
	if (keyData.keys[KEY_RIGHT_SHIFT] != NOT_PRESSED)
		return true;
	return false;
}

void EventController::setKey(InputCodes::Keyboard key, Keystate state)
{
	keyInputData.keys[key] = state;
}

void EventController::setMouseButton(InputCodes::Mouse button, Keystate state)
{
	keyInputData.mouseButtons[button] = state;
}

void EventController::setMousePosition(glm::vec2 pos)
{
	keyInputData.mousePosition = pos;
}
