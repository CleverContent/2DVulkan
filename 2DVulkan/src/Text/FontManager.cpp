#include "FontManager.h"


//Return Unicode values not purely ASCII values
std::map<unsigned long, GlyphData> FontManager::LoadFont()
{
	PublicVulkanData& publicVulkanData = *PublicVulkanData::getInstance();

	FontData& fontData = publicVulkanData.fontData;

	fontData.charsToGlyphIndex.clear();

	std::map<unsigned long, GlyphData> output{};

	std::string filename = std::filesystem::current_path().string() + "/src/Resources/Fonts/Stray-Regular.ttf";


	FT_Face face = GetFaceDataOfFont(filename);

	
	int counter = 0;
	FT_ULong charcode = -1;
	FT_UInt glyphIndex = -1;

	charcode = FT_Get_First_Char(face, &glyphIndex);
	while (glyphIndex != 0)
	{
		FT_Load_Glyph(face, glyphIndex, 0);

		FT_Outline outline = face->glyph->outline;

		std::vector<glm::vec2> points = GetGlyphPoints(outline);
		std::vector<char> tags = GetGlyphTags(outline);

		if (points.size() == 0)
		{
			charcode = FT_Get_Next_Char(face, charcode, &glyphIndex);
			continue;
		}


		std::vector<short> contours = GetGlyphContours(outline);

		std::vector<glm::vec2> expandedPoints{};

		int lastEndCounterIndex = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			std::vector<glm::vec2> p = ExpandBeizerPointsInContour(points, tags, lastEndCounterIndex, contours.at(i));
			if (i < contours.size() - 1)
			{
				p.push_back({ -999, -999 });
			}

			expandedPoints.insert(expandedPoints.end(), p.begin(), p.end());

			lastEndCounterIndex = contours.at(i) + 1;
		}

		ChangeBeizerPointsToBeGreaterThanZero(expandedPoints);

		GlyphData gd{};

		gd.GlyphID = counter;
		gd.unicode = charcode;
		gd.numberOfContours = contours.size();
		gd.points = expandedPoints;

		output.insert({ charcode, gd });
		fontData.charsToGlyphIndex.insert({charcode, counter});

		counter++;
		charcode = FT_Get_Next_Char(face, charcode, &glyphIndex);
	}

	return output;
}



std::map<unsigned long, GlyphData> FontManager::LoadFontTest()
{
	std::map<unsigned long, GlyphData> output{};

	GlyphData gd{};

	gd.GlyphID = 0;
	gd.numberOfContours = 1;
	gd.points = { {100, 0}, {200, 100}, {100, 200}, {1, 100}, {100, 0}};
	gd.unicode = 1;

	output.insert({1, gd});
	
	GlyphData gd1{};

	gd1.GlyphID = 1;
	gd1.numberOfContours = 3;
	gd1.points = { {100, 0}, {200, 50}, {100, 100}, {0, 50}, {100 ,0}, {-999, -999}, {100, 100}, {200, 150}, {100, 200}, {0, 150}, {100, 100}, {-999, -999}, {25, 0}, {50, 25}, {25, 50}, {0, 25}, {25, 0} };
	gd1.unicode = 1;

	output.insert({ 2, gd1 });

	return output;
}


FT_Face FontManager::GetFaceDataOfFont(std::string fontFilePath)
{
	FT_Library  library;
	FT_Face     face;
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		std::runtime_error("Failed to init FreeType");
	}

	error = FT_New_Face(library,
		fontFilePath.c_str(),
		0,
		&face);

	FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	if (error == FT_Err_Unknown_File_Format)
	{
		throw std::runtime_error("Failed to Find That file or File is not supported");
	}

	else if (error)
	{
		throw std::runtime_error("Failed to Create New FreeType Face");
	}

	return face;
}

std::vector<glm::vec2> FontManager::ExpandBeizerPointsInContour(std::vector<glm::vec2> points, std::vector<char> tags, int startIndex, int endIndex)
{
	std::vector<glm::vec2> output{};

	for (int i = startIndex; i < endIndex; i++)
	{
		output.push_back(points.at(i));
		if (tags.at(i) == tags.at(i + 1))
		{
			output.push_back(GetMidPoint(points.at(i), points.at(i + 1)));
		}
	}
	output.push_back(points.at(endIndex));

	if (tags.at(endIndex) == tags.at(startIndex))
	{
		output.push_back(GetMidPoint(points.at(endIndex), points.at(startIndex)));
	}
	
	output.push_back(points.at(startIndex));

	return output;
}

void FontManager::ChangeBeizerPointsToBeGreaterThanZero(std::vector<glm::vec2>& points)
{
	glm::vec2 smallestValue = {0, 0};

	for (glm::vec2 p : points)
	{
		if (p.x == -999 && p.y == -999)
		{
			continue;
		}

		if (p.x < smallestValue.x)
		{
			smallestValue.x = p.x;
		}	

		if (p.y < smallestValue.y)
		{
			smallestValue.y = p.y;
		}
			
	}

	for (glm::vec2& p : points)
	{
		p.x -= smallestValue.x;
		p.y -= smallestValue.y;
	}
}

std::vector<short> FontManager::GetGlyphContours(FT_Outline outline)
{
	std::vector<short> contours(outline.n_contours);
	for (int i = 0; i < outline.n_contours; i++)
	{
		contours.at(i) = *(outline.contours + i);
	}
	return contours;
}
std::vector<char> FontManager::GetGlyphTags(FT_Outline outline)
{
	std::vector<char> tags(outline.n_points);
	for (int i = 0; i < outline.n_points; i++)
	{
		tags.at(i) = *(outline.tags + i);
	}
	return tags;
}
std::vector<glm::vec2> FontManager::GetGlyphPoints(FT_Outline outline)
{
	std::vector<glm::vec2> points(outline.n_points);
	for (int i = 0; i < outline.n_points; i++)
	{
		FT_Vector vec = *(outline.points + i);
		points.at(i) = {vec.x, vec.y};
	}
	return points;
}

glm::vec2 FontManager::GetMidPoint(glm::vec2 p1, glm::vec2 p2)
{
	return { (p2.x + p1.x) / 2, (p2.y + p1.y) / 2 };
}
