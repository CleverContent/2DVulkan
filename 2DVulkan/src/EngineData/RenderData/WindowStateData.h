#pragma once
#include <map>
#include <string>

enum Windows
{
	WORLD,
	SPELLCREATOR,
	UNKNOWN
};

const static std::map<Windows, std::string> WindowStringNames =
{
	{WORLD, "World"},
	{SPELLCREATOR, "SpellCreator"}
};
