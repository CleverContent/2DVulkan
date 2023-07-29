#pragma once
#include <vector>
#include <map>
#include <functional>

#include <memory>
#include <glm.hpp>

#include "ConversionTable.h"
#include "EventData.h"
#include "KeyState.h"
#include "EngineData/EventData/Interface_OnClick.h"

struct OnClickMemeberFunction
{
	void* classPointer = nullptr;
};

struct Callbacks
{
	std::map<int, std::vector<void*>> onClickCallBacks{};

	void insertClickCallback(int mouseButton, void* callback)
	{
		if (onClickCallBacks.count(mouseButton) == 0)
		{
			onClickCallBacks.insert({ mouseButton, {} });
		}
		onClickCallBacks.at(mouseButton).push_back(callback);
	}

	void removeClickCallback(int mouseButton, void* callback)
	{
		if (onClickCallBacks.count(mouseButton) == 0)
		{
			return;
		}

		for (int i = 0; i < onClickCallBacks.at(mouseButton).size(); i++)
		{
			if (onClickCallBacks.at(mouseButton).at(i) == callback)
			{
				onClickCallBacks.at(mouseButton).erase(onClickCallBacks.at(mouseButton).begin() + i);
			}
		}
	}

	void checkClickCallbacks(int mouseButton, glm::vec2 mousePos)
	{
		for (auto [key, value] : onClickCallBacks)
		{
			if (key == mouseButton)
			{
				for (auto f : value)
				{
					((Interface_OnClick*)f)->OnClickCallBack(mousePos);
				}
			}
		}
	}
};

struct KeySet
{
	std::vector<InputCodes::Keyboard> keys;
	std::vector<InputCodes::Mouse> mouseButtons;
};

struct KeyInputData
{
	Keystate keys[InputCodes::KEY_UNDEFINED];
	Keystate mouseButtons[InputCodes::BUTTON_UNDEFINED];
	glm::vec2 mousePosition = { 0, 0 };

	Callbacks callbacks = {};
};
