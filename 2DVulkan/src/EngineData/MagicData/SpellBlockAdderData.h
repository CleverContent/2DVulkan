#pragma once
#include <vector>
#include <string>
#include <map>

#include "SpellBlocksDefinitions/BlockDataParent.h"
#include "EngineData/MagicData/SpellBlock.h"

struct SpellBlockAdderData
{
	VisualSpellBlock* currentSelectedBlock;
	bool placeBlock = false;
};