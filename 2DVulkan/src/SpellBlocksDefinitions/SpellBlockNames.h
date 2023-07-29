#pragma once
#include <string>
#include <vector>
#include "Event_OnClick.h"
#include "Action_CreateElement.h"
#include "EngineData/MagicData/SpellBlock.h"
#include "SpellBlocksDefinitions/BlockDataParent.h"
#include "SpellBlocksDefinitions/Action_CreateElement.h"
#include "SpellBlocksDefinitions/Event_OnClick.h"

inline const std::vector<std::string> getSpellBlockNames()
{
	std::vector<std::string> SpellBlockNames = {};
	SpellBlockNames.push_back("ON_CLICK EVENT");
	SpellBlockNames.push_back("CREATE_ELEMENT ACTION");

	return SpellBlockNames;
}

inline const VisualSpellBlock getBlockFromString(std::string)
{

}