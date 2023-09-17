#pragma once

static enum VertexDataTypes
{
	FLOAT,
	INT,
	VEC2,
	VEC3,
	VEC4
};

struct VertexDataInfo
{
	VertexDataTypes type = VEC2;
	int locationIndex = 0;
	int bindingIndex = 0;
};