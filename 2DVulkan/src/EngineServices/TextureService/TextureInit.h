#pragma once
#include <utility>
#include <algorithm>
#include <memory>

#include "EngineData/RenderData/VulkanData/Texture.h"
#include "EngineData/RenderData/VulkanData/GlyphTexture.h"
#include "EngineData/RenderData/VulkanData/CoreVulkanData.h"
#include "EngineServices/VulkanInitializerService.h"


#include "Text/FontManager.h"

struct BezierCurve
{
	glm::i16vec2 p1;
	glm::i16vec2 p2;
	glm::i16vec2 p3;

	glm::i16vec2 minCoords()
	{
		return { std::min(std::min(p1.x, p2.x), p3.x), std::min(std::min(p1.y, p2.y), p3.y) };
	}
	glm::i16vec2 maxCoords()
	{
		return { std::max(std::max(p1.x, p2.x), p3.x), std::max(std::max(p1.y, p2.y), p3.y) };
	}
};

struct BezierStruct
{
	BezierCurve points{};

	int xCoordInTexture = -1;//In Numerical type, Pure row counter
	int yCoordInTexture = -1;//In bytes, 0 for first, 4 for second because 4 bytes per float
};

struct BandData
{
	glm::i16 CurveCount = 0;
	glm::i16 locationDataOffset = -1;//In Bytes

	std::vector<glm::i16vec2> curveLocations{};

};

struct BandGroupData
{
	std::vector<BandData> bands{};

	BandGroupData(std::vector<BandData> HorizontalBands, std::vector<BandData> VerticalBands)
	{
		bands.insert(bands.end(), HorizontalBands.begin(), HorizontalBands.end());
		bands.insert(bands.end(), VerticalBands.begin(), VerticalBands.end());

		for (int i = 0; i < bands.size(); i++)
		{
			if (bands.at(i).CurveCount == 0)
			{
				bands.at(i).locationDataOffset = 0;
				continue;
			}

			int bytesToLocationData = 2;//For the initial two bytes to get to the end of the current Texel beacuse the offset bytes is in the y pos(2 of 4)
			for (int j = i + 1; j < bands.size(); j++)
			{
				bytesToLocationData += 8;//For the headers in front
			}

			for (int j = i - i; j >= 0; j--)
			{
				bytesToLocationData += bands.at(j).CurveCount * 8;//For the Location Data already added from behind Headers
			}

			bands.at(i).locationDataOffset = bytesToLocationData;
		}
	}
};

class TextureInit
{
public:
	static std::shared_ptr<Texture> CreateTextureForFont();
	static std::shared_ptr<GlyphTexture> CreateTextureForBezierLocation();

	static std::vector<BezierStruct> CreateBezierLocationList(GlyphData data);
	static std::vector<BezierCurve> CreateBezierList(GlyphData data);

	static std::pair<glm::vec2, glm::vec2> GetMinAndMax(std::vector<glm::i16vec2> bezierCurve);

	bool compareGlmi16Vec2Horizontal(glm::i16vec2 v1, glm::i16vec2 v2)
	{
		return (v1.x < v2.x);
	}
	bool compareGlmi16Vec2Vertical(glm::i16vec2 v1, glm::i16vec2 v2)
	{
		return (v1.y < v2.y);
	}
};