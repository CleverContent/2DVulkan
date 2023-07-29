#pragma once
#include <imgui.h>
#include <iostream>
#include "SpellBlocksDefinitions/Event_OnClick.h"

#include "MagicControllerFlags.h"
#include "EngineData/EventData/KeyInputData.h"
#include "EngineData/MagicData/MagicData.h"
#include "SpellBlocksDefinitions/SpellBlockNames.h"
#include "EngineData/MagicData/SpellBlockAdderData.h"

#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"

class MagicController
{
public:
	MagicController() {};
	MagicController(MagicControllerFlags MCF);
	~MagicController();

	void Init();

	void Update();

	void SpellCreatorImGui();

public:


private:

	MagicData magicData{};
	SpellBlockAdderData spellCreatorData{};
	Event_OnClick event_OnClick{};
};