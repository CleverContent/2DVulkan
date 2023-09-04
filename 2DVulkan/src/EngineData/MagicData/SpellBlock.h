#pragma once
#include "SpellBlocksDefinitions/BlockDataParent.h"
#include "BlockTypes.h"


//This is the "block" that gets placed in the spell creater when creating the spell and changing numbers
struct VisualSpellBlock
{
	uint32_t blockId = UINT32_MAX;
	
	BlockTypes blockType = UNKNOWNBLOCKTYPE;

	SpellBlock* spellBlock = nullptr;
};