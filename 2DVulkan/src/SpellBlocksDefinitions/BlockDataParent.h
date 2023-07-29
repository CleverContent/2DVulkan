#pragma once
#include <vector>
#include <glm.hpp>
#include <string>
#include "EngineData/MagicData/BlockTypes.h"
#include "EngineData//RenderData/VulkanData/PublicVulkanData.h"

enum DataTypes
{
	INT,
	FLOAT,
	BOOL,
	STRING,
	VECTOR2,
	VECTOR3
};



struct SpellActionData
{
	std::vector<int> v_Int = {};
	std::vector<float> v_Float = {};
	std::vector<bool> v_Bool = {};
	std::vector<std::string> v_String = {};
	std::vector<glm::vec2> v_Vec2 = {};
	std::vector<glm::vec3> v_Vec3 = {};

	PublicVulkanData* publicData = nullptr;

	SpellActionData()
	{

	}

	SpellActionData(SpellActionData& p)
	{
		v_Int = p.v_Int;
		v_Float = p.v_Float;
		v_Bool = p.v_Bool;
		v_String = p.v_String;
		v_Vec2 = p.v_Vec2;
		v_Vec3 = p.v_Vec3;
		publicData = p.publicData;
	}
};

struct SpellBlock
{
protected:
	virtual std::string getString() = 0;

	void* addnewTypeToData(DataTypes type)
	{
		switch (type)
		{
		case(INT):
			data.v_Int.push_back(-1);
			return &(data.v_Int.at(data.v_Int.size() - 1));

		case(FLOAT):
			data.v_Float.push_back(-1.0f);
			return &(data.v_Float.at(data.v_Float.size() - 1));

		case(BOOL):
			data.v_Bool.push_back(false);
			return &(data.v_Bool.at(data.v_Bool.size() - 1));

		case(STRING):
			data.v_String.push_back("UNDEFINED");
			return &(data.v_String.at(data.v_String.size() - 1));

		case(VECTOR2):
			data.v_Vec2.push_back({-1, -1});
			return &(data.v_Vec2.at(data.v_Vec2.size() - 1));

		case(VECTOR3):
			data.v_Vec3.push_back({-1, -1, -1});
			return &(data.v_Vec3.at(data.v_Vec3.size() - 1));

		default:
			break;
		}
	}

	SpellActionData data{};
};