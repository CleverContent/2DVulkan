#pragma once
#include <optional>
#include <string>

struct PhysicalDeviceProperties
{
	std::optional<uint32_t> graphicsIndex;
	std::optional<uint32_t> presentIndex;

	std::string deviceName = "Undefined";

	bool isComplete()
	{
		return graphicsIndex.has_value() && presentIndex.has_value();
	}
};