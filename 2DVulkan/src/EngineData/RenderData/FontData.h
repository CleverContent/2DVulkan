#pragma once
#include <map>
#include <memory>

struct FontData
{
	std::map<long, int> charsToGlyphIndex{};//Unicode to Index, ASCII values will work mostly I think

	std::shared_ptr<GlyphTexture> glyphTexture;
	std::shared_ptr<Texture> texture;
};

