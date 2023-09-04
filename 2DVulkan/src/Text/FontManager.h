#pragma once

#include <filesystem>

#include <iostream>

#include <vector>
#include <glm.hpp>
#include <gtc/type_precision.hpp>

#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <fstream>
#include <exception>
#include <limits>
#include "EngineData/RenderData/VulkanData/Texture.h"
#include <string>

#include "EngineData/RenderData/VulkanData/PublicVulkanData.h"

struct GlyphData
{
	std::vector<glm::f32vec2> points{};
	int GlyphID = -1;//This is the number of the glpyh in face/font read order

	int numberOfContours = -1;

	long unicode = -1;
};

class FontManager
{
public:
	//Return Unicode values not purely ASCII values
	static std::map<unsigned long, GlyphData> LoadFont();
	static std::map<unsigned long, GlyphData> FontManager::LoadFontTest();
	
private:
	static FT_Face GetFaceDataOfFont(std::string fontFilePath);

	static std::vector<glm::vec2> ExpandBeizerPointsInContour(std::vector<glm::vec2> points, std::vector<char> tags, int startIndex, int endIndex);
	static void ChangeBeizerPointsToBeGreaterThanZero(std::vector<glm::vec2>& points);

	static std::vector<short> GetGlyphContours(FT_Outline outline);
	static std::vector<char> GetGlyphTags(FT_Outline outline);
	static std::vector<glm::vec2> GetGlyphPoints(FT_Outline outline);

	static glm::vec2 GetMidPoint(glm::vec2 p1, glm::vec2 p2);
};