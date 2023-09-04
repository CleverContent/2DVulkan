#pragma once
#include "Texture.h"
#include <map>


struct GlyphTexture : Texture
{
	std::map<unsigned long, glm::i16vec2> glpyhDataStartingPosition{};
};